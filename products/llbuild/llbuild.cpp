//===-- llbuild.cpp - llbuild Frontend Utillity ---------------------------===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2015 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

#include "llbuild/Basic/Version.h"

#include "llbuild/Commands/Commands.h"

#include <cstdio>
#include <cstdlib>

using namespace llbuild;
using namespace llbuild::commands;

static void usage() {
  fprintf(stderr, "Usage: %s [--version] [--help] <command> [<args>]\n",
          getprogname());
  fprintf(stderr, "\n");
  fprintf(stderr, "Available commands:\n");
  fprintf(stderr, "  ninja       -- Run the Ninja subtool\n");
  fprintf(stderr, "  buildengine -- Run the build engine subtool\n");
  fprintf(stderr, "  buildfile   -- Run the build file subtool\n");
  fprintf(stderr, "\n");
  exit(0);
}

int main(int argc, const char **argv) {
  // Support use of llbuild as a replacement for ninja by indirecting to the
  // `ninja build` subtool when invoked under the name `ninja.
  if (strcmp(getprogname(), "ninja") == 0) {
    // We still want to represent ourselves as llbuild in output messages.
    setprogname("llbuild");

    std::vector<std::string> args;
    args.push_back("build");
    for (int i = 1; i != argc; ++i) {
      args.push_back(argv[i]);
    }
    return executeNinjaCommand(args);
  }

  // Expect the first argument to be the name of a subtool to delegate to.
  if (argc == 1 || std::string(argv[1]) == "--help")
    usage();

  if (std::string(argv[1]) == "--version") {
    // Print the version and exit.
    printf("%s\n", getLLBuildFullVersion().c_str());
    return 0;
  }

  // Otherwise, expect a command name.
  std::string command(argv[1]);
  std::vector<std::string> args;
  for (int i = 2; i != argc; ++i) {
    args.push_back(argv[i]);
  }

  if (command == "ninja") {
    return executeNinjaCommand(args);
  } else if (command == "buildengine") {
    return executeBuildEngineCommand(args);
  } else if (command == "buildfile") {
    return executeBuildFileCommand(args);
  } else {
    fprintf(stderr, "error: %s: unknown command '%s'\n", getprogname(),
            command.c_str());
    return 1;
  }
}
