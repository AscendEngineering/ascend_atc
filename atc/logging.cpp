#include "logging.h"
#include "utilities.h"
#include "spdlog/async.h"

void logging::setup_logging(){

    spdlog::init_thread_pool(8192, 1);
    
    //console logger
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    
    //file logger
    auto max_size = 1048576 * 5;
    auto max_files = 3;
    std::string filename = "logs/" + std::to_string(utilities::now_epoch()) + ".log";
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>( filename , max_size, max_files);
    
    std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
    auto logger = std::make_shared<spdlog::async_logger>("base", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::set_default_logger(logger);
}


