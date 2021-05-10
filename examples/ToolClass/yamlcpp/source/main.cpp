#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h> //注意带上 yaml-cpp/yaml.h 头文件

using namespace std;

void readYaml(const YAML::Node& node){
	if(node.IsMap()){//是map
		for(auto it = node.begin(); it != node.end(); ++it) {
		    cout << (it->first).as<string>() <<':';
		    if((it->first).as<string>() == "grades"){
		    	cout << endl;
		    }
		    readYaml(it->second);
		}
	}
	if(node.IsSequence()){//是数组
		for(size_t i = 0; i < node.size(); ++i) {
			cout << '\t';
			readYaml(node[i]);
		}
	}
	if(node.IsScalar()){//是纯量
		cout << node.as<string>() << endl;
	}
}

void fun1()
{
	//加载yml文件
	YAML::Node node = YAML::LoadFile("./config1.yml");
	//读取yml文件,这里输出到控制台
	readYaml(node);
}

//基础方法使用
void fun2()
{
	YAML::Node config = YAML::LoadFile("./config2.yml");

    cout << "Node type " << config.Type() << endl;
    cout << "skills type " << config["skills"].Type() << endl;

    cout << "name:" << config["name"].as<string>() << endl;
    cout << "sex:" << config["sex"].as<string>() << endl;
    cout << "age:" << config["age"].as<int>() << endl;

    cout << "skills c++:" << config["skills"]["c++"].as<int>() << endl;
    cout << "skills java:" << config["skills"]["java"].as<int>() << endl;
    cout << "skills android:" << config["skills"]["android"].as<int>() << endl;
    cout << "skills python:" << config["skills"]["python"].as<int>() << endl;

    for(YAML::const_iterator it= config["skills"].begin(); it != config["skills"].end();++it)
    {
        cout << it->first.as<string>() << ":" << it->second.as<int>() << endl;
    }

    YAML::Node test1 = YAML::Load("[1,2,3,4]");
    cout << " Type: " << test1.Type() << endl;

    YAML::Node test2 = YAML::Load("1");
    cout << " Type: " << test2.Type() << endl;

    YAML::Node test3 = YAML::Load("{'id':1,'degree':'senior'}");
    cout << " Type: " << test3.Type() << endl;

    std::ofstream fout("testconfig.xml");

    config["score"] = 100;

    fout << config;

    fout.close();

	std::ofstream fout2("configTest.yml");
	//设置配置文件node数据
	fout2 << config <<std::endl;
	
	fout2.close();

}

/*
bool YamlToJson(const YAML::Node& ynode, Json::Value& jnode) {
    try {
        if(ynode.IsScalar()) {
            Json::Value v(ynode.Scalar());
            jnode.swapPayload(v);
            return true;
        }
        if(ynode.IsSequence()) {
            for(size_t i = 0; i < ynode.size(); ++i) {
                Json::Value v;
                if(YamlToJson(ynode[i], v)) {
                    jnode.append(v);
                } else {
                    return false;
                }
            }
        } else if(ynode.IsMap()) {
            for(auto it = ynode.begin();
                    it != ynode.end(); ++it) {
                Json::Value v;
                if(YamlToJson(it->second, v)) {
                    jnode[it->first.Scalar()] = v;
                } else {
                    return false;
                }
            }
        }
    } catch(...) {
        return false;
    }
    return true;
}

bool JsonToYaml(const Json::Value& jnode, YAML::Node& ynode) {
    try {
        if(jnode.isArray()) {
            for(int i = 0; i < (int)jnode.size(); ++i) {
                YAML::Node n;
                if(JsonToYaml(jnode[i], n)) {
                    ynode.push_back(n);
                } else {
                    return false;
                }
            }
        } else if(jnode.isObject()) {
            for(auto it = jnode.begin();
                    it != jnode.end();
                    ++it) {
                YAML::Node n;
                if(JsonToYaml(*it, n)) {
                    ynode[it.name()] = n;
                } else {
                    return false;
                }
            }
        } else {
            ynode = jnode.asString();
        }
    } catch (...) {
        return false;
    }
    return true;
}
*/

int main(int argc,char** argv)
{
	cout << "Test fun1"<<endl;
	fun1();
	cout << "Test fun2"<<endl;
	fun2();
    return 0;
}
