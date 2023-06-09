# This Dockerfile builds the xeus based JoyLoL jupyter kernel

# See: https://mybinder.readthedocs.io/en/latest/tutorials/dockerfile.html

# we get compiled and installed versions of cppzmq, libzmq, nl-json, xeus,
# xeus-zmq, and xtl from stephengaito/xeus-kernel-builder
FROM stephengaito/xeus-kernel-builder:0.0.1

# Make sure python is python3
#
RUN apt-get update && \
    apt-get --yes install python-is-python3 python3-pip git

# juptyerlab-git does not *yet* work with jupyterlab version 4.x.x
# see: https://github.com/jupyterlab/jupyterlab-git/issues/1245
# So, for the moment, we pin jupyterlab to the latest pre v4.x.x
#
# ALAS jupyter-repo2docker ASSUMES a classic notbook interface...
# SO we use the old packages (and hence need to pin them)
#
RUN python3 -m pip install --no-cache-dir \
  jupyter-server==1.24.0 notebook==6.5.4 jupyterlab==3.6.4 jupyterlab-git

# Add the joylol user
#
ARG NB_USER=joylol
ARG NB_UID=1000
ENV USER ${NB_USER}
ENV NB_UID ${NB_UID}
ENV HOME /home/${NB_USER}

RUN adduser --disabled-password \
    --gecos "Default JoyLoL user" \
    --uid ${NB_UID} \
    ${NB_USER}

# Make sure the contents of our repo are in ${HOME}
#
COPY . ${HOME}

# we need to compile and install jeClass/jeMain (and eventually joylolKernel)
# from the src directory
#
WORKDIR ${HOME}/src
RUN ninja
WORKDIR ${HOME}

# Now make everthing in $HOME owned by joylol
#
USER root
RUN chown -R ${NB_UID} ${HOME}
USER ${NB_USER}
WORKDIR ${HOME}
