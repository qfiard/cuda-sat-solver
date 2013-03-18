/*
 * CNFParser.h
 *
 *  Created on: 17 mars 2013
 *      Author: quentinfiard
 */

#ifndef CNFPARSER_H_
#define CNFPARSER_H_

#include "types.h"
#include <string>

class CNFParser {
public:
	static formula parseFormulaFromFile(std::string file_path);
};

#endif /* CNFPARSER_H_ */
