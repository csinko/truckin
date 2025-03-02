# Truckin

Truckin is a file dump utility written in C that processes files and generates token summaries. Built for those who know you just keep truckin’ on, it handles your files efficiently, keeping the focus on the long haul.

## Features

- Counts tokens in text files—because sometimes the light’s all shinin’ on me, and other times I can barely see, but every detail matters.
- Skips binary files, avoiding unnecessary detours on your journey.
- Processes Gitignore files to keep your workload lean, trimming the excess baggage.
- Multi-threaded processing for improved performance, ensuring smooth sailing even when the road gets bumpy.
- Outputs both human-readable and JSON summaries, guiding you through what a long, strange trip it’s been with your data.

## Building

To build the `truckin` executable, run:

```bash
nix build .#truckin
```

Let the process flow smoothly—sometimes you got to play your hand just right to get it rolling.

## Running Tests

Tests are written in Python using pytest, working with the `truckin` executable to confirm everything’s on track.

Run them with:

```bash
nix run .#tests
```

Check your progress to stay on course. Takes time, you pick a place to go, and then you keep pushing forward.

## Local Development

For local development, a devShell provides `truckin` and `pytest`. Set it up right—because sometimes the cards ain’t worth a dime if you don’t lay ‘em down properly.

Enter the dev shell with:

```bash
nix develop
```

Build the executable in the shell:

```bash
make
```

Run tests with:

```bash
pytest --tkn ./result/bin/tkn test.py
```

Preparation is everything—get your tools aligned, and the rest falls into place.

## Usage

**tkn Usage Information**

**Usage:**

```bash
tkn [options] [patterns...]
```

**Options:**

- `-o <output_file>`: Write output to a file instead of the clipboard.
- `-v`: Enable verbose output.
- `-j`: Output summary in JSON format.
- `-n`: Disable colored output.
- `-h`: Display this help message.

**Examples:**

```bash
# Process a single file and display the token summary:
tkn file.txt
```

```bash
# Process all .txt files with JSON output:
tkn -j *.txt
```

```bash
# Process files and write output to a file:
tkn -o output.txt file1.txt file2.txt
```

The `tkn` executable scans files or patterns, counting tokens in text files while skipping binaries. With multi-threaded processing and output in human-readable or JSON format, it keeps your workflow steady. Truckin’, up to Buffalo or wherever your files take you, it’s all about staying on the move.

## License

Truckin is distributed under the MIT License. See [LICENSE](LICENSE) for details.
