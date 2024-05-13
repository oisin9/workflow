#include <stdio.h>
#include <string.h>
#include <string>
#include <workflow/WFTaskError.h>
#include <workflow/WFTaskFactory.h>
#include <workflow/StringUtil.h>

using namespace protocol;

int main()
{
	std::string url = "postgresql://localhost:5432/mydatabase?user=postgres&password=strongPasswd&sslmode=verify-full";
	ParsedURI uri;
	URIParser::parse(url, uri);

	// 打印解析结果
	printf("scheme: %s\n", uri.scheme);
	printf("host: %s\n", uri.host);
	printf("port: %s\n", uri.port);
	printf("path: %s\n", uri.path);
	printf("query: %s\n", uri.query);
	printf("fragment: %s\n", uri.fragment);
	printf("state: %d\n", uri.state);
	printf("error: %d\n", uri.error);
	printf("user info: %s\n", uri.userinfo);
	
	return 0;
}