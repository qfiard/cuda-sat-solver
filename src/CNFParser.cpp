/*
 * CNFParser.cpp
 *
 *  Created on: 17 mars 2013
 *      Author: quentinfiard
 */

#include "CNFParser.h"
#include <string>
#include <fstream>
#include <stdexcept>
#include <list>

static void skipLine(FILE* file)
{
	char c=' ';
	while(c!='\n' && fscanf(file,"%c", &c)!=EOF)
	{

	}
}

formula CNFParser::parseFormulaFromFile(std::string file_path)
{
	formula res;

	printf("Reading cnf file %s ...\n",file_path.c_str());

	FILE* file = fopen(file_path.c_str(),"r");

    if(file==NULL)
    {
        throw std::string("No such file : ")+file_path;
    }

	char lineType = 'c';
	fscanf(file,"%c", &lineType);

	while (lineType != 'p')
	{
		skipLine(file);
		fscanf(file,"%c", &lineType);
	}

	char problemType[3];
	fscanf(file," %3s", problemType);
	if(strcmp(problemType,"cnf")!=0)
	{
		throw new std::runtime_error("Invalid problem type");
	}

	fscanf(file, " %d %d", &res.nbOfVariables, &res.nbOfClauses);

	printf("Nb of variables : %d\n", res.nbOfVariables);
	printf("Nb of clauses : %d\n", res.nbOfClauses);

	skipLine(file);

	res.clauses = new clause[res.nbOfClauses];

	for(int i=0 ; i<res.nbOfClauses ; i++)
	{
		clause& clause = res.clauses[i];
		std::list<literal> listOfLiterals;

		while(true)
		{
			literal l;

			if(fscanf(file,"%d", &l) == EOF)
			{
				throw new std::runtime_error("Clause definition should end with 0");
			}
			if(l==0)
			{
				break;
			}
			listOfLiterals.push_back(l);
		}
		clause.length = listOfLiterals.size();
		clause.literals = new literal[clause.length];

		int j=0;
		for(std::list<literal>::iterator it = listOfLiterals.begin(); it != listOfLiterals.end(); it++)
		{
			clause.literals[j] = *it;
			j++;
		}
	}
	printf("CNF file was successfully read.\n\n");

	return res;
}
