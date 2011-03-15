class ParentClass
{
    public:
        typedef void (ParentClass::* callback_t)(int, char);
    public:

        ParentClass(): 
            preProcess(&ParentClass::preProcess_define), 
            process(&ParentClass::process_define), 
            postProcess(&ParentClass::postProcess_define) {}

        ParentClass(callback_t p1, callback_t p2, callback_t p3): 
            preProcess(p1), process(p2), postProcess(p3) {}

        void copyCallback(ParentClass& object)
        {
            preProcess = object.preProcess;
            process = object.process;
            postProcess = object.postProcess;
        }

        void preProcess_define(int no, char ch)    { printf("Parent's preProcess() %d-%c\n", no, ch); }
        void process_define(int no, char ch)       { printf("Parent's process() %d-%c\n", no, ch); }
        void postProcess_define(int no, char ch)   { printf("Parent's postProcess() %d-%c\n", no, ch); }

    public:
        void run(int no, char ch)
        {
            if ( preProcess != 0x00 )
                (this->*preProcess)(no, ch);
            if ( process != 0x00 )
                (this->*process)(no, ch);
            if ( postProcess != 0x00 )
                (this->*postProcess)(no, ch);
        }

    public:
        callback_t preProcess;
        callback_t process;
        callback_t postProcess;
}; // end - class ParentClass

class Child1 : public ParentClass
{
    public:
        Child1(): ParentClass(
                static_cast<ParentClass::callback_t>(&Child1::preProcess_define), 
                static_cast<ParentClass::callback_t>(&Child1::process_define), 
                static_cast<ParentClass::callback_t>(&Child1::postProcess_define)
                ) {};
        void preProcess_define(int no, char ch)    { printf("Child1's preProcess() %d-%c\n", no, ch); }
        void process_define(int no, char ch)       { printf("Child1's process() %d-%c\n", no, ch); }
        void postProcess_define(int no, char ch)   { printf("Child1's postProcess() %d-%c\n", no, ch); }
}; // end - class Child1

class Child2 : public ParentClass
{
    public:
        Child2(): ParentClass(
                static_cast<ParentClass::callback_t>(&Child2::preProcess_define), 
                static_cast<ParentClass::callback_t>(&Child2::process_define), 
                static_cast<ParentClass::callback_t>(&Child2::postProcess_define)
                ) {};
        void preProcess_define(int no, char ch)    { printf("Child2's preProcess() %d-%c\n", no, ch); }
        void process_define(int no, char ch)       { printf("Child2's process() %d-%c\n", no, ch); }
        void postProcess_define(int no, char ch)   { printf("Child2's postProcess() %d-%c\n", no, ch); }
}; // end - class Child2
