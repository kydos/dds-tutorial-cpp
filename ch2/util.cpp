// -- Custom Include
#include "util.hpp"
// -- Boost Include
#include <boost/program_options.hpp>
// -- Std C++ Include
#include <iostream>


const unsigned long int  ONE_SECOND = 1E9;
const unsigned int       DEFAULT_SAMPLE_NUM = 1E3;
const unsigned int       DEFAULT_SENSOR_ID = 0;
const TemperatureScale   DEFAULT_TEMP_SCALE = CELSIUS;
const float              T0 = 25.0F;
const float              DT = 15;
const float              H0 = 0.5F;
const float              DH = 0.2;

timespec msec2timespec(unsigned long long msec) {
  unsigned long int nsec = msec * 1E6;
  timespec ts = {0, nsec};
  // nsec has to be <= 999999999
  
  if (nsec >= ONE_SECOND) {
    ts.tv_sec = nsec / ONE_SECOND;
    ts.tv_nsec = nsec - (ts.tv_sec * ONE_SECOND);
  } 

  return ts;
}

TemperatureScale char2tempscale(char s) {
  TemperatureScale scale;
  switch (s) {
    case 'C': 
      scale = CELSIUS;
      break;
    case 'F':
      scale = FAHRENHEIT;
      break;
    case 'K':
      scale = KELVIN;
      break;
    default:
      throw std::range_error("Unknown Temperature Scale");
  }
  return scale;
}

char scale_name[3] = {'C', 'F', 'K'};

char tempscale2char(TemperatureScale scale) {
  return scale_name[scale];
}

std::ostream&
operator << (std::ostream& os, const TempSensorType& ts)
{
  os << "(id = " << ts.id  
     << ", temp = " << ts.temp
      << ", hum = " << ts.hum
     << ", scale = " << tempscale2char(ts.scale)
     << ")";
     
  return os;
}


namespace po = boost::program_options;

tssub_options parse_tssub_args(int argc, char* argv[]) {
  tssub_options opt;
  
  po::options_description desc("Available options for <tssub> are");
  desc.add_options()
    ("help", "produce help message")
    ("samples", po::value<unsigned int>(), "the number of samples that will be read")
    ("period", po::value<long long>(), "the polling period (in milli-sec > 0)")
    ("default", "runs the application with samples=1000 and period=1sec")
    ;
    
    try {  
      po::variables_map vm;
      po::store(po::parse_command_line(argc, argv, desc), vm);
      po::notify(vm);
    
      if (vm.count("help") || argc == 1) {
	std::cout << desc << "\n";
	exit(-1);
      }
        
      if (vm.count("samples")) {
	opt.samples = vm["samples"].as<unsigned int>();
      }
      if (vm.count("period")) {
	long long period = 
	  vm["period"].as<long long>(); 
	if (period < 0)
	  throw "period has to be >= 0";
	opt.period = msec2timespec(period);
      }
      if (vm.count("default")) {
	opt = {
	  DEFAULT_SAMPLE_NUM, 
	  {0, ONE_SECOND-1}
	};
      }
  }
  catch(...) {
    std::cout << desc << std::endl;
    throw;
  }
  return opt;
}

tspub_options parse_tspub_args(int argc, char* argv[]) {
 
  tspub_options opt;
  
  po::options_description desc("Available options for <tspub> are");
  desc.add_options()
    ("help", "produce help message")
    ("id", po::value<unsigned int>(), "the (non-negative) integer representing the sensor ID")
    ("scale", po::value<char>(), "the sensor temperature scale (C = CELSIUS, F = FAHRENHEIT, K = KELVIN)") 
    ("samples", po::value<unsigned int>(), "the (non-negative) number of samples that will be read")
    ("period", po::value<long long>(), "the polling period (in milli-sec > 0)")
    ("T0", po::value<float>(), "Temperature reference point")
    ("deltaT", po::value<float>(), "Temperature variation interval")
    ("H0", po::value<float>(), "Humidity reference point")
    ("deltaH", po::value<float>(), "Humidity variation interval")
    ("default", "runs the application with id=0, scale=C, samples=1000, period=1sec, T0=25, deltaT=15, H0=0.5, deltaH=0.2") 
    ;
    
    try {  
      po::variables_map vm;
      po::store(po::parse_command_line(argc, argv, desc), vm);
      po::notify(vm);
    
      if (vm.count("help") || argc == 1) {
	std::cout << desc << "\n";
	exit(-1);
      }
      
      if (vm.count("id")) {
	opt.id = vm["id"].as<unsigned int>();
      }
      
      if (vm.count("scale")) {
	char s = vm["scale"].as<char>();
	opt.scale = char2tempscale(s);
      }
      
      if (vm.count("samples")) {
	opt.samples = vm["samples"].as<unsigned int>();
      }
      
      if (vm.count("period")) {
	long long period = 
	  vm["period"].as<long long>(); 
	if (period < 0)
	  throw "period has to be >= 0";
	opt.period = msec2timespec(period);
      }
      if (vm.count("T0")) {
	opt.t0 = vm["T0"].as<float>();
      }
      if (vm.count("deltaT")) {
	opt.dt = vm["deltaT"].as<float>();
      }
      if (vm.count("H0")) {
	opt.h0 = vm["H0"].as<float>();
      }
      if (vm.count("deltaH")) {
	opt.dh = vm["deltaH"].as<float>();
      }
      if (vm.count("default")) {
	opt =  {
	  DEFAULT_SENSOR_ID,
	  DEFAULT_TEMP_SCALE,
	  DEFAULT_SAMPLE_NUM, 
	  {0, ONE_SECOND-1},
	  T0,
	  DT,
	  H0,
	  DH
	};
      }
    }
  catch(...) {
    std::cout << desc << std::endl;
    throw;
  }
  return opt;
}
