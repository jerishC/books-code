/*!
 * \file stereo_on_with_cd_command.h
 * \brief The Stereo_On_With_CD_Command class implementation. 
 * \author Jianlong Chen <jianlong99@gmail.com>
 * \date 2013-05-08
 */
/* $Id$ */

#ifndef STEREO_ON_WITH_CD_COMMAND_H
#define STEREO_ON_WITH_CD_COMMAND_H

#include <string>
#include "command.h"
#include "stereo.h"

class Stereo_On_With_CD_Command : public Command
{
public:
    Stereo_On_With_CD_Command(Stereo &stereo);
    virtual ~Stereo_On_With_CD_Command();

	virtual void execute();
	virtual void undo();
	virtual std::string to_string();

protected:
	Stereo &stereo_;
};

#endif /* STEREO_ON_WITH_CD_COMMAND_H */

