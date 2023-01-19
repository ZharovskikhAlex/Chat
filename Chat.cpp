#include <iostream>
#include <string>
#include "Chat.h"

void Chat::start()
{
	isChatWork_ = true;
}

std::shared_ptr<User>Chat::getUserByLogin(const std::string& login) const
{                                                                                 //�������� login ������������ �� ��� name
	for (auto& user : users_)
	{                                                                             //�������� ���� ������
		if (login == user.GetUserLogin())                                         //���� ������������ ������ �� login ����� �������� ������������� �������� GetUserLogin
			return std::make_shared<User>(user);                                  // �� �� ���������� shared-��������� �� ����� ������������ 
	}
	return nullptr;
}

std::shared_ptr<User>Chat::getUserByName(const std::string& name)const
{                                                                                  //���������� ���������� ����������� 
	for (auto& user : users_)
	{
		if (name == user.GetUserName())
			return std::make_shared<User>(user);                                   //���������� ���������
	}
	return nullptr;
}

void Chat::showLoginMenu()
{
	currentUser_ = nullptr;                                                         //������������ ����, ������� ������������, �������� nullptr
	char operation;

	do
	{
		std::cout << "1 - Login" << std::endl;
		std::cout << "2 - SignUp" << std::endl;
		std::cout << "0 - Shutdown" << std::endl;
		std::cout << "Enter your choice: ";
		std::cin >> operation;
		std::cout << std::endl;

		switch (operation)
		{
		case '1':
			login();                                                                 //������� ��� ������������ ������� � �������
			break;

		case '2':
			try
			{
				signUp();                                                            //��������������
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;									//����� ����������, ��������� � ������� signup
			}
			break;

		case '0':
			isChatWork_ = false;
			break;

		default:
			std::cout << "1 or 2..." << std::endl;
			break;

		}
	} while (!currentUser_ && isChatWork_);                                           //���� ��������, ���� �� ��������������� ������������, � ���� ��� ��������
}

void Chat::login()
{
	std::string login, password;
	char operation;

	do
	{
		std::cout << "login: ";
		std::cin >> login;
		std::cout << "password: ";
		std::cin >> password;

		currentUser_ = getUserByLogin(login);                                          //��������� �� ������������, ������� �����������������
		if (currentUser_ == nullptr || (password != currentUser_->GetUserPassword()))  //���� ������� ������������ ��� � �������, �� ���������� ������ ����������, ��� ���� �������� ������
		{
			currentUser_ = nullptr;
			std::cout << "login failed..." << std::endl;
			std::cout << "0 - exit or (any key) repeat: ";
			std::cin >> operation;

			if (operation == '0')
				break;
		}
	} while (!currentUser_);
}

void Chat::showChat()const
{
	std::string from;
	std::string to;
	std::cout << "---Chat---" << std::endl;

	for (auto& mess : messages_)
	{
		if (currentUser_->GetUserLogin() == mess.GetFrom() || currentUser_->GetUserLogin() == mess.GetTo() || mess.GetTo() == "all")	//���� ������� ������������
		{
			from = (currentUser_->GetUserLogin() == mess.GetFrom()) ? "me" : getUserByLogin(mess.GetFrom())->GetUserName();
			if (mess.GetTo() == "all")
			{																															//����, to ����� all, �� �������� ��������� ���� �������������
				to = "all";
			}
			else
			{
				to = (currentUser_->GetUserLogin() == mess.GetTo()) ? "me" : getUserByLogin(mess.GetTo())->GetUserName();               /*���� �� ������� ��� ������������ ����� to, �� ���������� ��������� ������ ����,
																																		���, �� �������� ��� ������������ � ����������� ��� �������� ���� to*/
			}
			std::cout << "Message from " << from << " to " << to << std::endl;
			std::cout << "text: " << mess.GetText() << std::endl;
		}
	}
	std::cout << "-----------" << std::endl;
}

void Chat::signUp()
{
	std::string login, password, name;
	double time;
	std::cout << "login: ";
	std::cin >> login;
	std::cout << "password: ";
	std::cin >> password;
	std::cout << "name: ";
	std::cin >> name;
	std::cout << "\n";

	if (getUserByLogin(login) || login == "all")                    //���� login- all, �� ������
	{
		throw UserLoginExp();
	}

	if (getUserByName(name) || name == "all")
	{                                                               //���� name - all, �� ������
		throw UserNameExp();
	}

	User user = User(login, password, name, time);                  // ������� ������ ������ User
	users_.push_back(user);                                      //��������� ������������ � ������
	currentUser_ = std::make_shared<User>(user);                    //������� ��������� �� �������� ������������
}

void Chat::showUserMenu()
{
	char operation;
	std::cout << "Hi, " << currentUser_->GetUserName() << std::endl;
	while (currentUser_)
	{
		std::cout << "\nMenu: \n1 - ShowChat \n2 - Add message \n3 - Users \n0 - Logout \n" << std::endl;
		std::cout << "Enter your choice: ";
		std::cin >> operation;
		std::cout << std::endl;

		switch (operation)
		{
		case '1':
			showChat();
			break;

		case '2':
			addMessage();
			break;

		case '3':
			showAllUsersName();
			break;

		case '0':
			currentUser_ = nullptr;
			break;

		default:
			std::cout << "unknown choice" << std::endl;
			break;
		}
	}
}

void Chat::addMessage()
{
	std::string to, text;

	std::cout << "To (name or all): ";
	std::cin >> to;
	std::cout << "Text: ";
	std::cin.ignore();

	getline(std::cin, text);

	if (!(to == "all" || getUserByName(to)))
	{                                                                                 //���� �������� ��������� ����, ���� ������� ������������, ������� ��� ���� � �������
		std::cout << "error send message: can't find" << to << std::endl;
		return;
	}

	if (to == "all")
		messages_.push_back(Message{ currentUser_->GetUserLogin(),"all",text });   /*�������� ������ ������ Message � ����������� �������� ��������
																					  ������ �� �������� �������������, ����������, � ���� ����������*/
	else
		messages_.push_back(Message{ currentUser_->GetUserLogin(),getUserByName(to)->GetUserLogin(),text });
}

void Chat::showAllUsersName()const
{
	std::cout << "---Users---" << std::endl;
	for (auto& user : users_)
	{
		std::cout << user.GetUserName();
		if (currentUser_->GetUserLogin() == user.GetUserLogin())
			std::cout << "(me)";
		std::cout << std::endl;

	}
	std::cout << "-----------" << std::endl;
}