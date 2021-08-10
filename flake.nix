{
  description = "Image viewers to use with find-command";
  
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };
  
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        customOverrides = self: super: {
          # Overrides go here
        };

        app = pkgs.stdenv.mkDerivation {
          name = "ximageviewer";
          src = ./.;
          nativeBuildInputs = with pkgs; [
            gnumake
          ];
          buildInputs = with pkgs; [
            xorg.libX11
            xorg.libxcb
            xorg.libXau
            xorg.libXdmcp
            zlib
            giflib
            libjpeg
            libpng
          ];
          makeFlags = [
            "DESTDIR=$(out)"
          ];
        };
        packageName = "ximageviewer";
      in {
        packages.${packageName} = app;

        defaultPackage = self.packages.${system}.${packageName};

        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [  ];
          inputsFrom = builtins.attrValues self.packages.${system};
        };
      }
    );
}
