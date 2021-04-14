/**
 * @file Config.h
 * @brief 配置类封装
 * @author klc
 * @date 2020-08-03
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_CONFIG_H__
#define __STIBEL_CONFIG_H__

#include <string>
#include "StiBel/Log/LoggerManager.h"

class Config {
public:
    typedef std::map<std::string, std::string> ParamsList;

    /**
     * @brief 创建对应参数名的配置参数
     * @param[in] name 配置参数名称
     * @param[in] description 参数描述
     */
    Config(const std::string& name,const std::string& description = "");
    ~Config();

    /**
     * @brief 根据参数名获取对应值
     */
    std::string getParams( const std::string &name );

    /**
     * @brief 重载[]方法
     */
    inline std::string operator[] ( const std::string &name ) {
        return this->getParams( name );
    }

    /**
     * @brief 将Params类型的转换为字符串格式输出
     */
    std::string toString();

    /**
     * @brief 返回配置参数名称
     */
    const std::string& getName() const { return m_name;}

    /**
     * @brief 返回配置参数的描述
     */
    const std::string& getDescription() const { return m_description;}

    /**
     * @brief 返回配置参数的类型
     */
    const std::string getTypeName() const { return m_type;}
private:
    /**
     * @brief 从json格式读取内容
     */
    void readConfFromJson(const std::string& json_file);

private:
    DECLARE_STATIC_LOGGERMANAGER(); 
    /// 配置参数的名称
    std::string m_name;
    /// 配置参数的描述
    std::string m_description;
    /// 配置参数的描述
    std::string m_type;

    // 用map保存的参数键值对
    ParamsList m_mapParams;
};
#endif // __STIBEL_CONFIG_H__

