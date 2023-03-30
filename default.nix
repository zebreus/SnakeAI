{ pkgs ? import <nixpkgs> { } }:

with pkgs;
pkgs.clangStdenv.mkDerivation {
  pname = "snakeai";
  version = "0.0.1";

  src = ./.;

  nativeBuildInputs = [
    cmake
    pkgconfig
  ];

  buildInputs = [
    glew
    glfw
  ];

  meta.mainProgram = "SnakeAI";
}
