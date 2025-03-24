#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <list>
#include <set>
#include <map>
#define TODO "TODO"
#define ONGOING "ONGOING"
#define DONE "DONE"
using namespace std;

struct employee_info;


struct task_info
{
    int priority;
    string status;
    string task_name;
    vector<employee_info> employees;
};

struct employee_info
{
    vector<task_info> task;
    string name;
    int done_task = 0;
};

void add_employee(vector<employee_info> &employee, string name)
{
    employee_info new_employee;
    new_employee.name = name;
    new_employee.task = {};
    new_employee.done_task = 0;
    employee.push_back(new_employee);
}

task_info add_task(vector<task_info> &task, string task_name, int priority)
{
    task_info new_task;
    new_task.priority = priority;
    new_task.task_name = task_name;
    new_task.status = TODO;
    task.push_back(new_task);
    return new_task;
}

void assign_task(vector<employee_info> &employee, vector<task_info> &task, string taskname, string name)
{
    int employee_check = 0;
    int task_check = 0;
    int temp_employee_index = -1;
    int temp_task_index = -1;

    for (int i = 0; i < employee.size(); i++)
    {
        if (employee[i].name == name)
        {
            employee_check = 1;
            temp_employee_index = i;
            break;
        }
    }

    for (int i = 0; i < task.size(); i++)
    {
        if (task[i].task_name == taskname)
        {
            task_check = 1;
            temp_task_index = i;
            task[i].status = ONGOING;
            break;
        }
    }

    if (employee_check && task_check)
    {
        employee[temp_employee_index].task.push_back(task[temp_task_index]);
        task[temp_task_index].employees.push_back(employee[temp_employee_index]);
    }
}

void finish_task(vector<employee_info> &employee, vector<task_info> &tasks, string taskname)
{
    for (int i = 0; i < tasks.size(); i++)
    {
        if (tasks[i].task_name == taskname)
        {
            tasks[i].status = DONE;
            break;
        }
    }

    for (int i = 0; i < employee.size(); i++)
    {
        for (int j = 0; j < employee[i].task.size(); j++)
        {
            if (employee[i].task[j].task_name == taskname)
            {
                employee[i].task[j].status = DONE;
                employee[i].done_task++;
            }
        }
    }
}

void sort_task(vector<task_info> &tasks)
{
    int n = tasks.size();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (tasks[j].priority > tasks[j + 1].priority)
            {
                swap(tasks[j], tasks[j + 1]);
            }
            else if (tasks[j].priority == tasks[j + 1].priority && tasks[j].task_name > tasks[j + 1].task_name)
            {
                swap(tasks[j], tasks[j + 1]);
            }
        }
    }
}

void sort_employee(vector<employee_info> &employees)
{
    int n = employees.size();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (employees[j].name > employees[j + 1].name)
            {
                swap(employees[j], employees[j + 1]);
            }
        }
    }
}

void report_ongoing(const vector<task_info> &task, const vector<employee_info> &employee)
{
    vector<task_info> sorted_task = task;
    vector<employee_info> sorted_employee = employee;
    sort_task(sorted_task);

    for (int i = 0; i < sorted_task.size(); i++)
    {
        sort_employee(sorted_task[i].employees);
        if (sorted_task[i].status == ONGOING)
        {
            cout << sorted_task[i].task_name << "(" << sorted_task[i].priority << ") : ";
        }
        for (int j = 0; j < sorted_task[i].employees.size(); j++)
        {

            if (j == sorted_task[i].employees.size() - 1)
            {
                cout << sorted_task[i].employees[j].name << endl;
            }
            else
            {
                cout << sorted_task[i].employees[j].name << ",";
            }
        }
    }
}

void report_employee(const vector<employee_info> employee, string name)
{
    employee_info temp;
    int count_ongoing_work = 0;
    for (int i = 0; i < employee.size(); i++)
    {
        if (employee[i].name == name)
        {
            temp = employee[i];
            break;
        }
    }

    cout << temp.name << " has done " << temp.done_task << " tasks" << endl;

    for (int i = 0; i < temp.task.size(); i++)
    {
        if (temp.task[i].status == ONGOING)
        {
            count_ongoing_work++;
        }
    }
    if (count_ongoing_work == 0)
    {
        cout << temp.name << " is currently not working on any tasks." << endl;
        return;
    }
    else if (count_ongoing_work != 0)
    {
        cout << temp.name << " is currently working on these tasks : " << endl;
    }
    for (int i = 0; i < temp.task.size(); i++)
    {
        if (temp.task[i].status == ONGOING)
        {
            cout << i + 1 << ". "<< temp.task[i].task_name << endl;
        }
    }
}

void report_all(const vector<task_info> task)
{
    int todo_task = 0, ongoing_task = 0, done_task = 0;

    for (int i = 0; i < task.size(); i++)
    {
        if (task[i].status == TODO)
        {
            todo_task++;
        }
        else if (task[i].status == ONGOING)
        {
            ongoing_task++;
        }
        else if (task[i].status == DONE)
        {
            done_task++;
        }
    }

    cout << "TODO : " << todo_task << endl;
    cout << "ONGOING : " << ongoing_task << endl;
    cout << "DONE : " << done_task << endl;
}

int main()
{
    vector<employee_info> employees;
    vector<task_info> tasks;
    string command;

    while (getline(cin, command))
    {
        size_t space_pos = command.find(' ');
        string cmd = command.substr(0, space_pos);
        string after_cmd = (space_pos == string::npos) ? "" : command.substr(space_pos + 1);

        if (cmd == "add_task")
        {
            size_t second_space = after_cmd.find(' ');
            string task_name = after_cmd.substr(0, second_space);
            int priority = stoi(after_cmd.substr(second_space + 1));
            add_task(tasks, task_name, priority);
            cout << "task " << task_name << " added successfully" << endl;
        }
        else if (cmd == "add_employee")
        {
            add_employee(employees, after_cmd);
            cout << "Employee " << after_cmd << " added successfully" << endl;
        }
        else if (cmd == "assign_employee")
        {
            size_t second_space = after_cmd.find(' ');
            string task_name = after_cmd.substr(0, second_space);
            string name = after_cmd.substr(second_space + 1);
            assign_task(employees, tasks, task_name, name);
            cout << task_name << " has successfuly assigned to " << name << " successfully!" << endl;
        }
        else if (cmd == "finish_task")
        {
            finish_task(employees, tasks, after_cmd);
            cout << after_cmd << " has successfuly finished" << endl;
        }
        else if (cmd == "report")
        {
            if (after_cmd == "all")
            {
                report_all(tasks);
            }
            else if (after_cmd == "ongoing")
            {
                report_ongoing(tasks, employees);
            }
            else if (after_cmd.find("employee") == 0)
            {
                string name = after_cmd.substr(9);
                report_employee(employees, name);
            }
            else
            {
                cout << "Unknown command" << endl;
            }
        }
        else
        {
            cout << "Unknown command" << endl;
        }
    }
    return 0;
}
