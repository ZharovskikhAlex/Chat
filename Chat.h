#pragma once
#include <vector>
#include <exception>
#include <memory>
#include "Message.h"

struct UserLoginExp :public std::exception 
{
	                                               
	const char* what() const noexcept override		//вызываем класс исключений, в случае, если пользователя all
	{
		return "error: user login is busy";
	}

};

struct UserNameExp :public std::exception 
{
	const char* what()const noexcept override		 //âûçûâàåì êëàññ èñêëþ÷åíèé, â ñëó÷àå , åñëè ïîëüçîâàòåëü all
	{
		return "error: user name is busy";
	}
};

class Chat
{
	bool isChatWork_ = false;                      //ïåðåìåííàÿ, îòâå÷àþùàÿ çà ðàáîòó ÷àòà
	std::vector<User> users_;                   //ñîçäàåì âåêòîð, êîòîðûé õðàíèò ïîëüçîâàòåëåé ÷àòà
	std::vector<Message> messages_;             //ñîçäàåì âåêòîð, êîòîðûé õðàíèò ñîîáùåíèÿ â ÷àòå
	std::shared_ptr<User> currentUser_ = nullptr;  //shared óêàçàòåëü óêàçûâàåò íà òåêóùåãî ïîëüçîâàòåëÿ

	void login();
	void signUp();
	void showChat()const;
	void showAllUsersName()const;
	void addMessage();

	std::vector<User>& getAllUsers()
	{
		return users_;
	}
	std::vector<Message>& getAllMessages()
	{
		return messages_;
	}
	std::shared_ptr<User>getUserByLogin(const std::string& login)const;
	std::shared_ptr<User>getUserByName(const std::string& name)const;

public:
	void start();
	bool isChatWork()const
	{
		return isChatWork_;
	}
	std::shared_ptr<User>getCurrentUser()const
	{
		return currentUser_;
	}
	void showLoginMenu();
	void showUserMenu();
};
