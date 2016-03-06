//
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <uhd/types/tune_request.hpp>
#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <fstream>
#include <csignal>
#include <complex>
#include <stdio.h>

namespace po = boost::program_options;


int UHD_SAFE_MAIN(int argc, char *argv[]) {

	uhd::set_thread_priority_safe();

	// Setup program options
	std::string args, tx_ant, rx_ant, subdev, ref, wirefmt;
	size_t spb;
	double samp_rate, freq, tx_gain, rx_gain;

	po::options_description desc("Allowed Options:");
	desc.add_options()
		("help", "help message")
		("args", po::value<std::string>(&args)->default_value(""), "Multi UHD device args")
		("spb", po::value<size_t>(&spb)->default_value(10000), "Samples per Buffer")
		("samp_rate", po::value<double>(&samp_rate)->default_value(1e6), "Sample Rate")
		("freq", po::value<double>(&freq)->default_value(0.0), "RF Center Frequency (Hz)")
		("tx_gain", po::value<double>(&tx_gain)->default_value(0.0), "TX Gain")
		("rx_gain", po::value<double>(&rx_gain)->default_value(0.0), "RX Gain")
		("tx_ant", po::value<std::string>(&tx_ant), "TX Antenna")
		("rx_ant", po::value<std::string>(&rx_ant), "RX Antenna")
		("subdev", po::value<std::string>(&subdev), "Subdevice Specification")
		("ref", po::value<std::string>(&ref)->default_value("internal"), "Reference Source (internal, external")
		("wirefmt", po::value<std::string>(&wirefmt)->default_value("sc16"), "Wire Format (sc8, sc16)")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << boost::format("UHD Bent Pipe Example - %s") % desc << std::endl;
		std::cout << std::endl << "Receives samples and then retransmits them." << std::endl;
		return ~0;
	}


}