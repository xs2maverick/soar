#ifndef OUTPUT_LINK_MANAGER_H
#define OUTPUT_LINK_MANAGER_H

#include "sml_Client.h"

#include <list>
#include <string>

class Command
{
public:
	Command() {}
	Command( sml::Identifier* commandId );

	enum CommandType { MOVE, ROTATE, STOP, MOVE_TO, ROTATE_TO, BROADCAST_MESSAGE, REMOVE_MESSAGE };
	CommandType 	get_type();

	enum MoveDirection { MOVE_STOP, MOVE_FORWARD, MOVE_BACKWARD };
	MoveDirection 	get_move_direction();

	enum RotateDirection { ROTATE_STOP, ROTATE_LEFT, ROTATE_RIGHT };
	RotateDirection get_rotate_direction();

	double 			get_throttle();

	double get_x();
	double get_y();
	
	double get_a();
	
	const std::list< std::pair< std::string, std::string > > get_sentence() const;
	
	int get_remove_message_id() const;

	enum Status { STATUS_NONE, STATUS_COMPLETE, STATUS_EXECUTING, STATUS_ERROR };
	Status 			get_status();
	void 			set_status( Status status );
	
private:
	CommandType m_type;
	MoveDirection m_move;
	RotateDirection m_rotate;
	double m_throttle;
	Status m_status;
	double m_x;
	double m_y;
	double m_a;
	std::list< std::pair< std::string, std::string > > m_sentence;
	int m_remove_message_id;
};

class OutputLinkManager
{
public:
	OutputLinkManager( sml::Agent& agent );
	~OutputLinkManager();

	void read();
	Command* get_next_command();

	void commit();

private:
	std::list< std::pair< Command, sml::Identifier* > > m_command_list;
	std::list< std::pair< Command, sml::Identifier* > >::iterator m_command_iter;
	
	sml::Agent& m_agent;
};

#endif // OUTPUT_LINK_MANAGER_H

