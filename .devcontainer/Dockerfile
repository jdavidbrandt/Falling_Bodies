FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04
LABEL maintainer="Joshua Brandt <jdbrandt92@gmail.com>"
ARG KEY_NAME="id_ed25519"

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.22.2"

COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh



#Barebones install
RUN apt-get update && \
    apt-get install -y sudo curl gnupg gnuplot \
    build-essential git locales nodejs wget neovim npm fonts-powerline && \
    locale-gen en_US.UTF-8 && \
    adduser --quiet --disabled-password \
    --shell /bin/sh --home /home/devuser/ \
    --gecos "User" devuser
    
#Build and install raylib with dependencies
RUN apt-get update && \
    apt-get install -y libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev \
    libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev && \
    git clone https://github.com/raylib-extras/raylib-quickstart.git my3dcube

# The gnuplot-qt version doesn't work with hyprland, before I do this, try to see if I can get it working in wayland
# RUN apt-get remove -y gnuplot-qt

#docker run --device /dev/dri:/dev/dri

#The fix to gnuplot-qt I don't think it's runargs and containerenv, i think that can be removed in devcontainer, i think you have to install qtwayland5

#sudo apt install qtwayland5

##This is the lines that I'm not sure I need in devcontainer.json between "build" and "features"
##
##"runArgs":[
##        "--device=/dev/dri:/dev/dri",
##       "--volume=${localEnv:XDG_RUNTIME_DIR}/${localEnv:WAYLAND_DISPLAY}:/tmp/wayland-0"],
##    "containerEnv": {
##        "XDG_RUNTIME_DIR": "/tmp",
##        "WAYLAND_DISPLAY": "wayland-0",
##        "QT_QPA_PLATFORM": "wayland;xcb"
##    },