/*
 *  yosys -- Yosys Open SYnthesis Suite
 *
 *  Copyright (C) 2020  The Symbiflow Authors
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include "set_false_path.h"
#include <regex>
#include "kernel/log.h"
#include "sdc_writer.h"

USING_YOSYS_NAMESPACE

void SetFalsePath::help() {
    //   |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
    log("\n");
    log("   set_false_path [-quiet] [-from <net_name>] [-to <net_name>] \n");
    log("\n");
    log("Set false path on the specified net\n");
    log("\n");
    log("Print the output to stdout too. This is useful when all Yosys is "
        "executed.\n");
    log("\n");
    log("    -quiet\n");
    log("        Don't print the result of the execution to stdout.\n");
    log("\n");
    log("    -from\n");
    log("        List of start points or clocks.\n");
    log("\n");
    log("    -to\n");
    log("        List of end points or clocks.\n");
    log("\n");
}

void SetFalsePath::execute(std::vector<std::string> args,
                           RTLIL::Design* design) {
    RTLIL::Module* top_module = design->top_module();
    if (top_module == nullptr) {
	log_cmd_error("No top module detected\n");
    }

    size_t argidx;
    bool is_quiet = false;
    std::string from_pin;
    std::string to_pin;

    // Parse command arguments
    for (argidx = 1; argidx < args.size(); argidx++) {
	std::string arg = args[argidx];
	if (arg == "-quiet") {
	    is_quiet = true;
	    continue;
	}

	if (arg == "-from" and argidx + 1 < args.size()) {
	    from_pin = args[++argidx];
	    log("From: %s\n", from_pin.c_str());
	    continue;
	}

	if (arg == "-to" and argidx + 1 < args.size()) {
	    to_pin = args[++argidx];
	    log("To: %s\n", to_pin.c_str());
	    continue;
	}

	if (arg.size() > 0 and arg[0] == '-') {
	    log_cmd_error("Unknown option %s.\n", arg.c_str());
	}

	break;
    }
    if (!is_quiet) {
	std::string msg = (from_pin.empty()) ? "" : "-from " + from_pin;
	msg += (to_pin.empty()) ? "" : " -to " + to_pin;
	log("Adding false path %s\n", msg.c_str());
    }
    sdc_writer_.AddFalsePath(FalsePath{.from_pin = from_pin, .to_pin = to_pin});
}
