#ifndef CUSTOM_LOG_INFO_REDIRECTION_HPP  
#define CUSTOM_LOG_INFO_REDIRECTION_HPP  
#include <iostream>  
#include <functional>  
#include <string>  
#include <optional>  
#include <type_traits>  
#include <filesystem>  
#include <ostream>  
#include <fstream>  
#include <chrono>  
#include <ctime>
#include "DataStructures.hpp"  
#define _CRT_SECURE_NO_WARNINGS

#define maybe_unused [[maybe_unused]]

namespace utils {  
	template<typename T>  
	constexpr auto M_getSize(T _T) { return ( sizeof(_T) ) ; }  
}  

namespace i_Logger {  
// Removed `mutable` keyword as it is not allowed for static variables  
static const bool gErrorMayOccurBit = false;  

//may want to remove statics  
//error types   
static enum class gLogLevel { None = 0x200, Notification = 0x201, Warning = 0x202, Error = 0x203 };  
//target types   
static enum class gTargetOut { Console, UI };  
//console types   
static enum class gConsoleType { Buffered, Unbuffered };  
//Log class which will redirect the output to the proper source   
class Logger {  
private:  
	//default output is console   
	maybe_unused gTargetOut ltarget{ gTargetOut::Console };  
	//None signifies the plain message which is not an error   
	maybe_unused gLogLevel lloglevel{ gLogLevel::None };  
public:  
	Logger() = default;
	//e_input = external input function which can be UI's writetext() function for example  
	//u_msg = user message   
	static void WriteToTheOutput(const std::function<void(std::string&)>& e_input ,std::string& u_msg , gTargetOut u_targ ) {  
		e_input(std::ref(u_msg));  
	}  

	static void WriteToTheOutput(const std::function<void(const gConsoleType& , std::string&)>& e_input ,std::string& u_msg , gTargetOut u_targ , gConsoleType u_constype ) {  
		if (u_targ == gTargetOut::Console) {  
			e_input(u_constype, u_msg);  
		}  
	}  
};  

template <typename _Ty = std::string>  
void WriteToTheConsole(const gConsoleType& e_constype , _Ty e_str) {  
	if (std::is_same_v< decltype(e_str), std::string >) {  
		switch (e_constype) {  
		case gConsoleType::Buffered :   
			std::cout << e_str << std::endl;  
			break;  
		case gConsoleType::Unbuffered :  
			std::cerr << e_str << std::endl;  
			break;  
		}  
	}  
	else {  
		//for developer to understand where the problem may lay   
		// Removed `mutable` keyword from `gErrorMayOccurBit`  
		const_cast<bool&>(gErrorMayOccurBit) = true;  
		return;  
	}  
}  
// auto file write operation  
void WriteLogToTheFile(const std::filesystem::path& e_file_path, std::string_view& e_msg ,const uint8_t& wcode) {  
	namespace chrono = std::chrono;  
	try {  
		std::ofstream file{ e_file_path , std::ios_base::app };  
		if (!file) throw std::filesystem::filesystem_error("File creation failed", std::error_code());  

		auto curr_time = chrono::system_clock::now();  
		time_t now = chrono::system_clock::to_time_t(curr_time);  
		struct tm local; 

		localtime_s(&local, &now);
		char buffer[15];  

		file << "The log's being generated at - "   
			<< asctime_s(buffer , sizeof(buffer) , &local ) << std::endl  
			<< "Error has been detected - " << e_msg << " With the code - " << wcode << std::endl;  

	}  
	catch (std::filesystem::filesystem_error& ferr) {  
		std::cerr << ferr.what() << "On the - " << ferr.path1() << "/" << ferr.path2() << std::endl;  
		throw;  
	}  
}  
}  
#endif // CUSTOM_LOG_INFO_REDIRECTION_HPP