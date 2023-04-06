{
  description = "Snake ai";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-22.11";
    flake-utils.url = "github:numtide/flake-utils";
    nixgl = {
      url = "github:guibou/nixgl";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = { self, nixpkgs, flake-utils, nixgl }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        nixglPkgs = nixgl.packages.${system};
      in
      rec {
        name = "snakeai";
        packages.snakeai = import ./default.nix { pkgs = nixpkgs.legacyPackages.${system}; };
        packages.wrapped = pkgs.writeShellScriptBin "snakeai" ''
          ${nixglPkgs.default}/bin/nixGL ${packages.snakeai}/bin/SnakeAI
        '';
        packages.default = packages.wrapped;
      }
    );
}
