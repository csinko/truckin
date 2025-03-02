import json
import subprocess
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List, Optional

import pytest


@dataclass
class FileSummary:
    name: str
    tokens: int
    percent: float

    @classmethod
    def from_dict(cls, d: Dict[str, Any]) -> "FileSummary":
        return cls(
            name=d.get("name", ""),
            tokens=d.get("tokens", 0),
            percent=d.get("percent", 0.0)
        )


@dataclass
class TknSummary:
    total_files: int
    total_tokens: int
    files: List[FileSummary]

    @classmethod
    def from_dict(cls, d: Dict[str, Any]) -> "TknSummary":
        files = [FileSummary.from_dict(item) for item in d.get("files", [])]
        return cls(
            total_files=d.get("total_files", 0),
            total_tokens=d.get("total_tokens", 0),
            files=files
        )


@pytest.fixture
def tkn_executable(pytestconfig) -> Path:
    tkn_path = pytestconfig.getoption("--tkn")
    if not tkn_path:
        pytest.skip("tkn executable not provided")
    # Resolve to an absolute path
    p = Path(tkn_path).resolve()
    if not p.exists():
        pytest.skip(f"tkn executable does not exist at {p}")
    return p


def run_tkn(
    executable: Path,
    args: List[str],
    cwd: Optional[Path] = None,
    input_text: Optional[str] = None
) -> Any:
    """Run the tkn executable with given arguments in an optional working directory."""
    result = subprocess.run(
        [str(executable)] + args,
        capture_output=True,
        text=True,
        cwd=str(cwd) if cwd else None,
        input=input_text,
    )
    return result


def parse_json_summary(output: str) -> TknSummary:
    """
    Parse the JSON summary output from tkn into a TknSummary instance.
    Expected JSON format:
    {
        "total_files": <int>,
        "total_tokens": <int>,
        "files": [
            {"name": "<filename>", "tokens": <int>, "percent": <float>},
            ...
        ]
    }
    """
    data = json.loads(output)
    return TknSummary.from_dict(data)


def test_single_text_file(tmp_path: Path, tkn_executable: Path) -> None:
    # Create a single text file with known content.
    file_content = "Hello world"
    test_file = tmp_path / "test.txt"
    test_file.write_text(file_content)

    # Run tkn with JSON output mode.
    result = run_tkn(tkn_executable, ["-j", str(test_file)], cwd=tmp_path)
    assert result.returncode == 0, f"Executable failed: {result.stderr}"

    summary = parse_json_summary(result.stdout)
    assert summary.total_files == 1, "Expected 1 file in JSON summary."
    # "Hello world" yields 2 tokens.
    assert summary.total_tokens == 2, "Expected 2 tokens in JSON summary."

    assert len(summary.files) == 1, "Expected 1 file entry in JSON summary."
    file_entry = summary.files[0]
    # The file name may be empty or "test.txt" depending on how the base path is computed.
    assert file_entry.name in {"", "test.txt"}, f"Unexpected file name: {file_entry.name}"
    assert file_entry.tokens == 2, "File token count mismatch."


def test_binary_file(tmp_path: Path, tkn_executable: Path) -> None:
    # Create a binary file (with null bytes) so that it is skipped.
    binary_content = b"\x00\x01\x02\x03\x04"
    binary_file = tmp_path / "binary.bin"
    binary_file.write_bytes(binary_content)

    result = run_tkn(tkn_executable, ["-j", str(binary_file)], cwd=tmp_path)
    summary = parse_json_summary(result.stdout)
    assert summary.total_tokens == 0, "Binary file token count should be 0."


def test_no_file_found(tmp_path: Path, tkn_executable: Path) -> None:
    # Use a pattern that does not match any file.
    nonexistent_pattern = str(tmp_path / "nonexistent*")
    result = run_tkn(tkn_executable, [nonexistent_pattern], cwd=tmp_path)
    assert "No files found." in result.stderr, "No file found case not handled properly."


def test_output_file_summary(tmp_path: Path, tkn_executable: Path) -> None:
    # Create several text files with unique contents.
    files: Dict[str, str] = {
        "file1.txt": "Content of file one.",
        "file2.txt": "Second file content goes here.",
        "file3.txt": "The quick brown fox jumps over the lazy dog."
    }
    expected_tokens = {}
    for fname, content in files.items():
        file_path = tmp_path / fname
        file_path.write_text(content)
        expected_tokens[fname] = len(content) // 4

    # Run tkn in JSON mode.
    args = ["-j"] + list(files.keys())
    result = run_tkn(tkn_executable, args, cwd=tmp_path)
    assert result.returncode == 0, f"Executable failed: {result.stderr}"

    summary = parse_json_summary(result.stdout)
    assert summary.total_files == len(files), "Mismatch in number of files."
    assert len(summary.files) == len(files), "Mismatch in file entries count."

    for file_entry in summary.files:
        fname = file_entry.name
        tokens = file_entry.tokens
        if fname in expected_tokens:
            assert tokens == expected_tokens[fname], f"Token count mismatch for {fname}."


def test_gitignore_processing(tmp_path: Path, tkn_executable: Path) -> None:
    # Create a subdirectory with a .gitignore that excludes one file.
    subdir = tmp_path / "subdir"
    subdir.mkdir()
    (subdir / ".gitignore").write_text("exclude.txt\n")
    (subdir / "include.txt").write_text("Include this file")
    (subdir / "exclude.txt").write_text("Do not include")

    result = run_tkn(tkn_executable, ["-j", str(subdir)], cwd=tmp_path)
    summary = parse_json_summary(result.stdout)
    names = {entry.name for entry in summary.files}
    assert "include.txt" in names, "Included file missing in JSON summary."
    assert "exclude.txt" not in names, "Excluded file appeared in JSON summary."


def test_large_number_of_files(tmp_path: Path, tkn_executable: Path) -> None:
    # Create many files with a large number of lines to force multi-threaded processing.
    num_files = 50
    num_lines = 1000  # Large number of lines per file
    file_contents: Dict[str, str] = {}
    for i in range(num_files):
        fname = f"file_{i:03d}.txt"
        # Create a file with 1,000 lines. Each line is unique for that file.
        lines = [f"Line {j} for file {i:03d}" for j in range(num_lines)]
        content = "\n".join(lines)
        file_contents[fname] = content
        (tmp_path / fname).write_text(content)

    result = run_tkn(tkn_executable, ["-j", str(tmp_path)], cwd=tmp_path)
    assert result.returncode == 0, f"Executable failed: {result.stderr}"
    summary = parse_json_summary(result.stdout)
    assert summary.total_files >= num_files, f"Expected at least {num_files} files, found {summary.total_files}"
    assert len(summary.files) == num_files, f"Expected {num_files} file entries, got {len(summary.files)}"

    for entry in summary.files:
        fname = entry.name
        tokens = entry.tokens
        expected = len(file_contents.get(fname, "")) // 4
        assert tokens == expected, f"Token count mismatch for {fname}: expected {expected}, got {tokens}"

