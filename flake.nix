{
  description = "Truckin: A file dump utility";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        versionFile = builtins.readFile ./version.h;
        versionMatch = builtins.match ".*#define TRUCKIN_VERSION \"([^\"]+)\".*" versionFile;
        version = if versionMatch != null then builtins.head versionMatch else throw "Unable to parse version.h";

        truckin = pkgs.stdenv.mkDerivation {
          inherit version;
          pname = "truckin";
          src = ./.;
          buildPhase = "make";
          installPhase = "make install PREFIX=$out";
        };

        testEnv = pkgs.python3.withPackages (ps: [ ps.pytest ]);
      in {
        packages.truckin = truckin;

        apps.tests = {
          type = "app";
          program = "${pkgs.bash}/bin/bash -c 'exec ${testEnv}/bin/pytest --tkn ${truckin}/bin/tkn test.py'";
        };

        devShells.default = pkgs.mkShell {
          buildInputs = [
            truckin
            testEnv
          ];
        };
      }
    );
}
