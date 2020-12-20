/*# This file is part of the
  #   ___ _       __    __      _  __ 
  #  / _ \ |_  __/ / /\ \ \___ | |/ _|
  # / /_)/ \ \/ /\ \/  \/ / _ \| | |_ 
  #/ ___/| |>  <  \  /\  / (_) | |  _|
  #\/    |_/_/\_\  \/  \/ \___/|_|_|  
  # project
  # https://github.com/jacmoe/pxlwolf
  #
  # (c) 2020 Jacob Moena
  #
  # BSD 2-Clause "Simplified" License
  #
*/
#include "utils.h"
#include "olcPixelGameEngine.h"

namespace moena {
	std::string utils::get_homedir(void)
	{
		std::string homedir = "";
#ifdef _WIN32
		homedir = std::getenv("HOMEDRIVE");
		homedir.append(std::getenv("HOMEPATH"));
#else
		homedir = std::getenv("HOME");
#endif
		return homedir;
	}
}
