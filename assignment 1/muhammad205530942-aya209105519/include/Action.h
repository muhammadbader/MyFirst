    #ifndef ACTION_H_
    #define ACTION_H_

    #include <string>
    #include <iostream>

    class Session;
    class User;

    enum ActionStatus{
        PENDING, COMPLETED, ERROR
    };


    class BaseAction{
    public:
        BaseAction();
        virtual ~BaseAction();
        BaseAction(const BaseAction&);
        ActionStatus getStatus() const;
        virtual void act(Session& sess)=0;
        virtual std::string toString() const=0;
        virtual void erroract(Session& sess);
        virtual BaseAction *clone()=0;
        virtual void setkind(std::string);
        virtual void setstatus(ActionStatus);
        virtual void seterror(std::string);
        virtual std::string geterr()const;
        virtual ActionStatus getsta()const;

    protected:
        void complete();
        void error(const std::string &errorMsg);
        std::string getErrorMsg() const;
    private:
        std::string errorMsg;
        ActionStatus status;
        std::string kind;
    };

    class CreateUser  : public BaseAction {
    public:
        CreateUser();
        CreateUser(const CreateUser& other);
        virtual CreateUser& operator= (const CreateUser& other);
        virtual void act(Session& sess);
        virtual std::string toString() const;
        virtual BaseAction *clone();
    };

    class ChangeActiveUser : public BaseAction {
    public:
        ChangeActiveUser();
        ChangeActiveUser(const ChangeActiveUser& other);
        virtual ChangeActiveUser& operator= (const ChangeActiveUser& other);
        virtual void act(Session& sess);
        virtual std::string toString() const;
        virtual BaseAction *clone();
    };

    class DeleteUser : public BaseAction {
    public:
        DeleteUser();
        DeleteUser(const DeleteUser& other);
        virtual DeleteUser& operator= (const DeleteUser& other);
        virtual void act(Session & sess);
        virtual std::string toString() const;
        virtual BaseAction *clone();
    };


    class DuplicateUser : public BaseAction {
    public:
        DuplicateUser();
        DuplicateUser(const DuplicateUser& other);
        virtual DuplicateUser& operator= (const DuplicateUser& other);
        virtual void act(Session & sess);
        virtual std::string toString() const;
        virtual BaseAction *clone();
    };

    class PrintContentList : public BaseAction {
    public:
        PrintContentList();
        PrintContentList(const PrintContentList& other);
        virtual PrintContentList& operator= (const PrintContentList& other);
        virtual void act (Session& sess);
        virtual std::string toString() const;
        virtual BaseAction *clone();
    };

    class PrintWatchHistory : public BaseAction {
    public:
        PrintWatchHistory();
        PrintWatchHistory(const PrintWatchHistory& other);
        virtual PrintWatchHistory& operator= (const PrintWatchHistory& other);
        virtual void act (Session& sess);
        virtual std::string toString() const;
        virtual BaseAction *clone();
    };


    class Watch : public BaseAction {
    public:
        Watch();
        Watch(const Watch& other);
        virtual Watch& operator= (const Watch& other);
        virtual void act(Session& sess);
        virtual std::string toString() const;
        virtual BaseAction *clone();
    };


    class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        PrintActionsLog(const PrintActionsLog& other);
        virtual PrintActionsLog& operator= (const PrintActionsLog& other);
        virtual void act(Session& sess);
        virtual std::string toString() const;
        virtual BaseAction *clone();
    };

    class Exit : public BaseAction {
    public:
        Exit();
        Exit(const Exit& other);
        virtual Exit& operator= (const Exit& other);
        virtual void act(Session& sess);
        virtual std::string toString() const;
        virtual BaseAction *clone();
    };
    #endif
