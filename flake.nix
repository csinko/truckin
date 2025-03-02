{
  description = "Truckin: A file dump utility";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        # Define the project source directory.
        project = ./.;

        versionFile = builtins.readFile ./version.h;
        versionMatch = builtins.match ".*#define TRUCKIN_VERSION \"([^\"]+)\".*" versionFile;
        version = if versionMatch != null then builtins.head versionMatch else throw "Unable to parse version.h";

        truckin = pkgs.stdenv.mkDerivation {
          inherit version;
          pname = "truckin";
          src = project;
          buildPhase = "make";
          installPhase = "make install PREFIX=$out";
        };

        testEnv = pkgs.python3.withPackages (ps: [ ps.pytest ]);

        # Define a test runner script
        testRunner = pkgs.writeShellScriptBin "run-tests" ''
          #!/bin/sh
          cd ${project} || { echo "Failed to cd to ${project}"; exit 1; }
          exec ${testEnv}/bin/pytest -p no:cacheprovider --tkn ${truckin}/bin/tkn ${project}/test.py
        '';
      in {
        packages.truckin = truckin;
        packages.default = truckin;

        apps.tests = {
          type = "app";
          program = "${testRunner}/bin/run-tests";
        };

        devShells.default = pkgs.mkShell {
          src = project;
          buildInputs = [
            truckin
            testEnv
          ];
        };
      }
    );
}
