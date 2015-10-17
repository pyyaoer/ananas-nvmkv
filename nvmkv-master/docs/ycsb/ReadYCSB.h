#ifndef READYCSB_H
#define READYCSB_H
#include <vector>
#include <cstring>
#include <string>
#include <errno.h>

namespace ReadYCSB
{
	void insert(const char *key, const char *value)
	{
		printf("%s %s\n", key, value);
	}

	void update(const char *key, const char *value)
	{
		insert(key,value);
	}
	
	void read(const char *key, const char *value)
	{
	}
	
	struct node
	{
		std::string key;
		std::string value;
		int type;
	};

	std::vector<node> parse(const char *file_name)
	{
		std::vector<node> ret;
		FILE *fin = fopen(file_name, "r");
		if (fin == NULL){
			printf("errno: %d\n", errno);
		}
		// skip first several lines
		for (int i=0;i<3;++i)
		{
			fscanf(fin, " %*[^\n]");
		}
		
		bool context = false;
		while (1)
		{
			char line[5000];
			if (fscanf(fin, " %[^\n]", line) < 1)
			{
				break;
			}
			if (line[0] < 'A' || line[0] > 'Z')
			{
				if (context)
				{
					break;
				}
				else
				{
					//context = true;
				}
				continue;
			}
			context = true;
			char key[100], value[2000];
			int type;
			if (line[0] == 'I')
			{
				type = 0;
			}
			else if (line[0] == 'U')
			{
				type = 1;
			}
			else if (line[0] == 'R')
			{
				type = 2;
			}
			else
			{
				type = -1;
			}
			sscanf(line, "%*s %*s %s", key);
			if (line[0] == 'I' || line[0] == 'U')
			{
				strcpy(value, strstr(line, "field") + 7);
				value[strlen(value) - 2] = '\0';
			}
			else
			{
				value[0] = '\0';
			}
			node n;
			n.key = key;
			n.value = value;
			n.type = type;
			ret.push_back(n);
		}
		fclose(fin);
		return ret;

	}

}

#endif
