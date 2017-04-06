/////////////////////////////////////////////////////////////////////////////////////////
#ifndef CONFIG_HPP
#define CONFIG_HPP
/////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <utility>
/////////////////////////////////////////////////////////////////////////////////////////
namespace tools
{
/////////////////////////////////////////////////////////////////////////////////////////
enum class open_mode
{
    read,
    write,
    readwrite
};
/////////////////////////////////////////////////////////////////////////////////////////
class config
{
    //-----------------------------------------------------------------------------------
    using key_value = std::pair<std::string, std::string>;
    //-----------------------------------------------------------------------------------
public:
    //-----------------------------------------------------------------------------------
    config( const open_mode mode = open_mode::read );
    //-----------------------------------------------------------------------------------
    ~config();
    //-----------------------------------------------------------------------------------
    bool load_file( const std::string& file_name );
    //-----------------------------------------------------------------------------------
    bool save_file();
    //-----------------------------------------------------------------------------------
    template<typename _Tp>
    void get( const std::string& key, _Tp& input ) const;
    //-----------------------------------------------------------------------------------
    template<typename _Tp>
    void get( const std::string& key, std::vector<_Tp>& input ) const;
    //-----------------------------------------------------------------------------------
    template<typename _Tp>
    void set( const std::string& key, const _Tp& value );
    //-----------------------------------------------------------------------------------
    template<typename _Tp>
    void set( const std::string& key, const std::vector<_Tp>& value );
    //-----------------------------------------------------------------------------------
private:
    //-----------------------------------------------------------------------------------
    template<typename _Tp>
    std::string type_2_str( const _Tp value ) const;
    //-----------------------------------------------------------------------------------
    template<typename _Tp>
    _Tp str_2_type( const std::string& value ) const;
    //-----------------------------------------------------------------------------------
    bool read_file();
    //-----------------------------------------------------------------------------------
    bool write_file() const;
    //-----------------------------------------------------------------------------------
    key_value parse_line( const std::string& line ) const;
    //-----------------------------------------------------------------------------------
    std::map<std::string, std::string> data;
    open_mode mode;
    std::string file_name;
    bool is_changed;
    //-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
#define _Template \
    template<typename _Tp>
/////////////////////////////////////////////////////////////////////////////////////////
_Template
inline void config::get( const std::string& key, _Tp& input ) const
{
    if( mode == open_mode::write )
        return;
    
    auto it = data.find(key);
    
    if( it != data.end() )
    {
        input = str_2_type<_Tp>(it->second);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
_Template
inline void config::get( const std::string& key, std::vector<_Tp>& input ) const
{
    if( mode == open_mode::write )
        return;
    
    auto it = data.find(key);
    
    if( it != data.end() )
    {
        input.clear();
        
        std::istringstream strm{ it->second };
        std::string tmp{};
        
        while( std::getline( strm, tmp, ',' ) )
        {
            input.emplace_back(str_2_type<_Tp>(tmp));
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
_Template
inline void config::set( const std::string& key, const _Tp& value )
{
    if( mode == open_mode::read )
        return;
    
    data[key] = type_2_str<_Tp>(value);
    is_changed = true;
}
/////////////////////////////////////////////////////////////////////////////////////////
_Template
inline void config::set( const std::string& key, const std::vector<_Tp>& value )
{
    if( mode == open_mode::read )
        return;
    
    std::string result;
    
    for( size_t it{}; it < value.size() - 1; ++it )
    {
        result += type_2_str<_Tp>(value[it]) + ",";
    }
    result += type_2_str<_Tp>(value.back());
    
    data[key] = result;
    is_changed = true;
}
/////////////////////////////////////////////////////////////////////////////////////////
_Template
inline std::string config::type_2_str( const _Tp value ) const
{
    throw "No known conversion.";
}
/////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string config::type_2_str<std::string>( const std::string value ) const
{
    return value;
}
/////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string config::type_2_str<int>( const int value ) const
{
    std::ostringstream strm{};
    strm << value;
    return strm.str();
}
/////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string config::type_2_str<double>( const double value ) const
{
    std::ostringstream strm{};
    strm << value;
    return strm.str();
}
/////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string config::type_2_str<bool>( const bool value ) const
{
    return value ? "true" : "false";
}
/////////////////////////////////////////////////////////////////////////////////////////
_Template
inline _Tp config::str_2_type( const std::string& value ) const
{
    throw "No known conversion.";
}
/////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string config::str_2_type<std::string>( const std::string& value ) const
{
    return value;
}
/////////////////////////////////////////////////////////////////////////////////////////
template<>
inline int config::str_2_type<int>( const std::string& value ) const
{
    std::istringstream strm{ value };
    int ret{};
    strm >> ret;
    
    return ret;
}
/////////////////////////////////////////////////////////////////////////////////////////
template<>
inline double config::str_2_type<double>( const std::string& value ) const
{
    std::istringstream strm{ value };
    double ret{};
    strm >> ret;
    
    return ret;
}
/////////////////////////////////////////////////////////////////////////////////////////
template<>
inline bool config::str_2_type<bool>( const std::string& value ) const
{
    return value == "true" || value == "1" ? true : false;
}
/////////////////////////////////////////////////////////////////////////////////////////
#undef _Template
/////////////////////////////////////////////////////////////////////////////////////////
} //tools
/////////////////////////////////////////////////////////////////////////////////////////
#endif // CONFIG_HPP
/////////////////////////////////////////////////////////////////////////////////////////
