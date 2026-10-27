# Building with Docker
If you dont already know what docker is [read this](https://docs.docker.com/get-started/docker-overview/), but in short docker
allows you to to build this project without the hassle of dependencies and environmental incompatibilities.
## Requirements
You will have to install docker desktop in order to use docker this [Setup Guide](https://github.com/WCSCourses/index/blob/main/Docker_guide.md)
will help you install it on any platform. You will also have to setup the iso folder as described in the [ReadMe](../README.md#requirements).
## Quick Start
(NOTE: For Mac M series laptops / arm64 laptops use `docker buildx build --platform linux/amd64 -t sotc .`)
```sh
# This sets up the docker container
docker build -t sotc .                          

# This will compile the all module with cmake
docker compose run --rm sotc
```
## Using individual commands
(NOTE: Please review [CMake documentation](CMAKE.md) or [MakeFile documentation](MAKEFILE.MD) for a list of commands)
### Example:
```sh
# For this example i will be using docker compose because its platform independent easier to type than raw docker  
docker compose run --rm sotc "cmake --preset default, cmake --build out, make progress, make clean"
```
To use individual commands instead of the default type  `docker compose run --rm sotc` followed by your command wrapped in quotes,
to run multiple command separate them with commas.
