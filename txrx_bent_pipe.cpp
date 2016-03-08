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

static bool stop_signal_called = false;
void sig_int_handler(int){stop_signal_called = true;}

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
		("tx_ant", po::value<std::string>(&tx_ant)->default_value("TX/RX"), "TX Antenna")
		("rx_ant", po::value<std::string>(&rx_ant)->default_value("RX2"), "RX Antenna")
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


	// Create the USRP Device
	std::cout << boost::format("Creating the USRP Device with: %s...") % args << std::endl;
	uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(args);

	// Set reference
	usrp->set_clock_source(ref);

	std::cout << boost::format("Using Device: %s") % usrp->get_pp_string() <<std::endl;

	// Set the Sample Rate
	if (samp_rate <= 0.0) {
		std::cerr << "Please specify a valid sample rate..." << std::endl;
		return ~0;
	}

	std::cout << boost::format("Setting RX Rate: %s Msps...") % (samp_rate/1e6) << std::endl;
	usrp->set_rx_rate(samp_rate);
	std::cout << boost::format("Setting TX Rate: %s Msps...") % (samp_rate/1e6) << std::endl;
	usrp->set_tx_rate(samp_rate);

	std::cout << boost::format("Actual RX Rate: %s Msps...") % (usrp->get_rx_rate()/1e6) << std::endl;
	std::cout << boost::format("Actual TX Rate: %s Msps...") % (usrp->get_tx_rate()/1e6) << std::endl;
	std::cout << std::endl;

	// Set Center Frequency
	std::cout << boost::format("Setting Center Freq: %f MHz") % (freq/1e6) << std::endl;
	uhd::tune_request_t tune_request(freq);
	usrp->set_rx_freq(tune_request);
	std::cout << boost::format("Actual RX Freq: %f MHz") % (usrp->get_rx_freq()/1e6) << std::endl;
	std::cout << boost::format("Actual TX Freq: %f MHz") % (usrp->get_tx_freq()/1e6) << std::endl;
	std::cout << std::endl;

	// Set the Gain
	std::cout << boost::format("Setting RX Gain: %f dB") % (rx_gain) << std::endl;
	usrp->set_rx_gain(rx_gain);
	std::cout << boost::format("Setting TX Gain: %f dB") % (tx_gain) <<std::endl;
	usrp->set_tx_gain(tx_gain);

	std::cout << boost::format("Actual RX Gain: %f dB") % (usrp->get_rx_gain()) << std::endl;
	std::cout << boost::format("Actual TX Gain: %f dB") % (usrp->get_tx_gain()) << std::endl;
	std::cout << std::endl;

	// Set Antennas
	usrp->set_rx_antenna(rx_ant);
	usrp->set_tx_antenna(tx_ant);

	// Sleep to allow USRP to setup
	boost::this_thread::sleep(boost::posix_time::seconds(1.0));

	std::signal(SIGINT, &sig_int_handler);
	std::cout << "Press Ctrl + C to stop streaming..." << std::endl;

	//Create the USRP Streamers and Threads...


}