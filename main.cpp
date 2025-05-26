#include <string>
#include <iostream>
#include <fstream>
#include <codecvt>

#ifdef WIN32
#include <Windows.h>
std::string utf8ToGBK_winAPI(const std::string& utf8Str) {
    // 1. 计算将UTF-8转换为宽字符（UTF-16）所需的缓冲区大小
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
    if (wideLen <= 0) {
        throw std::runtime_error("UTF-8 转宽字符失败");
    }

    // 2. 分配宽字符缓冲区并进行转换
    std::wstring wideStr(wideLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wideStr[0], wideLen);

    // 3. 计算将宽字符（UTF-16）转换为GBK所需的缓冲区大小
    int gbkLen = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (gbkLen <= 0) {
        throw std::runtime_error("宽字符转GBK失败");
    }

    // 4. 分配GBK缓冲区并进行转换
    std::string gbkStr(gbkLen, 0);
    WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &gbkStr[0], gbkLen, nullptr, nullptr);

    // 5. 去掉字符串末尾的空字符
    gbkStr.pop_back();
    return gbkStr;
}
#endif

class codecvt_gbk : public std::codecvt_byname<wchar_t, char, std::mbstate_t>
{
public:
    codecvt_gbk()
#ifdef WIN32
        :codecvt_byname("zh_CN")
#else
        : codecvt_byname("zh_CN.GBK")
#endif
    {
    }
};

static std::wstring_convert<codecvt_gbk> s_GbkConvert;
static std::wstring_convert<std::codecvt_utf8<wchar_t>> s_Utf8Convert;


std::string Utf8ToGbk(const std::string& str)
{
    return s_GbkConvert.to_bytes(s_Utf8Convert.from_bytes(str));
}

int main(int argc, char* argv[])
{
    for(int i=0; i<argc; i++){
        std::cout<<"argc_"<<i<<": "<<argv[i]<<std::endl;
    }

    if(argc < 2){
        std::cout<<"argc("<<argc<<") < 2"<<std::endl;
        return 1;
    }
    std::string filepath = argv[1];

    std::cout<<"filepath:"<<filepath<<std::endl;

    std::ifstream ifs(filepath);
    if(!ifs.is_open()){
        std::cerr<<"ifs.is_open failed."<<std::endl;
        std::cerr<<"filepath:"<<filepath<<std::endl;
        return 3;
    }

    int num = 0;
    std::string str;
    while(std::getline(ifs, str)){
        std::cout<<"line_"<<num++<<std::endl;
        std::cout<<"origin string:\n"<<str<<std::endl;
#ifdef  WIN32
        std::cout<<"Utf8ToGbk_codecvt(str2):\n"<<Utf8ToGbk(str)<<std::endl;
        std::cout<<"Utf8ToGBK_winAPI(str2):\n"<<utf8ToGBK_winAPI(str)<<std::endl;
#endif
        std::cout<<std::endl;
    }
    ifs.close();

    
    return 0;
}