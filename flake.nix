{
  description = "hiper development environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShells.default =
          pkgs.mkShell.override
            {
              stdenv = pkgs.clangStdenv;
            }
            {
              packages =
                with pkgs;
                [
                  clang
                  bear
                  gnumake
                ]
                ++ (if system == "aarch64-darwin" then [ ] else [ gdb ]);

              shellHook = ''
                export LD_LIBRARY_PATH="${pkgs.lib.makeLibraryPath [ ]}:$LD_LIBRARY_PATH"
              '';
            };

        packages.default = 1;
      }
    );
}
