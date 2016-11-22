/////////////////////////////////////////////////////////////////////////////////////////
#include "config.hpp"
#include <fstream>
/////////////////////////////////////////////////////////////////////////////////////////
namespace tools
{
/////////////////////////////////////////////////////////////////////////////////////////
config::config( const open_mode mode )
    :
    mode(mode),
    is_changed(false)
{}
/////////////////////////////////////////////////////////////////////////////////////////
config::~config()
{
    if( mode != open_mode::read )
        save_file();
}
/////////////////////////////////////////////////////////////////////////////////////////
bool config::load_file( const std::string& _file_name )
{
    file_name = _file_name;
    return read_file();
}
/////////////////////////////////////////////////////////////////////////////////////////
bool config::save_file()
{
    if( is_changed )
    {
        is_changed = false;
        return write_file();
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////
bool config::read_file()
{
    std::ifstream f_in{ file_name };
    
    if( !f_in )
        return false;
    
    std::string tmp{};
    while( std::getline(f_in, tmp) )
    {
        auto pair = parse_line(tmp);
        
        if( !pair.first.empty() )
        {
            data[pair.first] = pair.second;
        }
    }
    
    is_changed = false;
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////
bool config::write_file() const
{
    std::vector<key_value> file_cont;
    
    std::ifstream f_in{ file_name };
    
    if( f_in )
    {
        std::string tmp{};
        while( std::getline(f_in, tmp) )
        {
            auto pair = parse_line(tmp);
            
            if( !pair.first.empty() )
            {
                auto it = data.find(pair.first);
                
                if( it != data.end() )
                {
                    pair.second = it->second;
                }
            }
            else
            {
                pair.first = tmp;
            }
            file_cont.emplace_back( pair );
        }
    }
    else
    {
        for( const auto& it : data )
        {
            file_cont.emplace_back(std::make_pair(it.first, it.second));
        }
    }
    
    f_in.close();
    
    std::ofstream f_out( file_name );
    if( !f_out )
    {
        return false;
    }
    for( const auto& it : file_cont )
    {
        f_out << it.first;
        
        if( !it.second.empty() )
            f_out << "=" << it.second;
        
        f_out << std::endl;
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////
std::pair<std::string, std::string> config::parse_line( const std::string& line ) const
{
    if( !line.empty() && line.find_first_of("#;") == std::string::npos )
    {
        size_t idx{};
        
        while( std::isspace(line[idx]) )
            ++idx;
        
        const size_t key_idx = idx;
        
        while( !std::isspace(line[idx]) && line[idx] != '=' )
            ++idx;
        
        const std::string key = line.substr(key_idx, idx - key_idx);
        
        while( std::isspace(line[idx]) || line[idx] == '=' )
            ++idx;
        
        return std::make_pair(key, line.substr(idx, line.size() - idx));
    }
    return std::make_pair(std::string{}, std::string{});
}
/////////////////////////////////////////////////////////////////////////////////////////
} //tools
/////////////////////////////////////////////////////////////////////////////////////////