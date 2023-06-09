#include <memory>
#include <algorithm>  // std:algorithms defines std::find

#include "xeus/xkernel.hpp"
#include "xeus/xkernel_configuration.hpp"
#include "xeus-zmq/xserver_zmq.hpp"

#include "jeClass.hpp"

// Since we have very simple collection of command line options...
// we use @iain's simple example (public domain) code for command line parsing
// see: https://stackoverflow.com/a/868894

char* getCmdOption(char ** begin, char ** end, const std::string& option) {
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

int main(int argc, char* argv[]) {

  // deal with a request for the command line help
  if (cmdOptionExists(argv, argv+argc, "-h")) {
    // print the help text then exit
    return 0;
  }

  // deal with a request to write out the kernel.json file
  // see: https://jupyter-client.readthedocs.io/en/stable/kernels.html#kernel-specs
  char* kernelFilePath = getCmdOption(argv, argv+argc, "-j");
  if (kernelFilePath) {
    // write out the kernel file
    return 0;
  }

  // deal with the "normal" jupyter call
  const char* fileName = getCmdOption(argv, argv+argc, "-f");
  if (!fileName) {
    fileName = (argc == 1) ? "connection.json" : argv[2];
  }
  if (!fileName) {
    // print an error message and then exit
    return 1;
  }

  xeus::xconfiguration config = xeus::load_configuration(std::string(fileName));

  auto context = xeus::make_context<zmq::context_t>();

  using interpreter_ptr = std::unique_ptr<jsonEcho::JsonEcho>;
  interpreter_ptr interpreter = interpreter_ptr(new jsonEcho::JsonEcho());

  xeus::xkernel kernel(
    config, xeus::get_user_name(),
    std::move(context), std::move(interpreter),
    xeus::make_xserver_zmq
  );
  kernel.start();

  return 0;
}