#include <iostream>
#include "Jobs.hpp"
using namespace std;

void printTestedInfo(string func_name) {
    cout << "Tested " << func_name << endl;
}


bool verbose = false;
// 测试添加作业
void testAddJob() {
    Jobs& jobs = Jobs::getInstance();

    // 添加一个合法的作业
    bool result = jobs.addJob(1234, FG, "ls -l");
    std::cout << "Test 1: Add job (valid PID) - Result: " << (result ? "Pass" : "Fail") << std::endl;

    // 尝试添加一个非法的作业（PID 小于 1）
    result = jobs.addJob(0, BG, "echo Hello");
    std::cout << "Test 2: Add job (invalid PID) - Result: " << (!result ? "Pass" : "Fail") << std::endl;

    // 添加多个作业，检查是否超出最大作业数
    const int MAX_JOBS = 16;
    for (int i = 1; i < MAX_JOBS; ++i) {
        result = jobs.addJob(i + 1, BG, "sleep 1");
    }
    result = jobs.addJob(MAX_JOBS + 1, BG, "sleep 1"); // 添加第 MAX_JOBS+1 个作业
    std::cout << "Test 3: Add too many jobs - Result: " << (!result ? "Pass" : "Fail") << std::endl;

    // 检查作业是否按预期添加
    std::cout << "Test 4: Check added jobs:" << std::endl;
    jobs.list(); // 打印作业列表，检查是否添加正确

    printTestedInfo(__func__);
}


void testDeleteJob() {
    Jobs& jobs = Jobs::getInstance();

    // 添加几个作业
    jobs.addJob(1234, FG, "ls -l");
    jobs.addJob(5678, BG, "echo Hello");

    // 删除一个存在的作业
    bool result = jobs.deleteJob(1234);
    std::cout << "Test 1: Delete existing job - Result: " << (result ? "Pass" : "Fail") << std::endl;

    // 尝试删除一个不存在的作业
    result = jobs.deleteJob(9999);
    std::cout << "Test 2: Delete non-existing job - Result: " << (!result ? "Pass" : "Fail") << std::endl;

    // 再次删除已删除的作业
    result = jobs.deleteJob(1234);
    std::cout << "Test 3: Delete already deleted job - Result: " << (!result ? "Pass" : "Fail") << std::endl;

    // 检查作业列表是否正确
    std::cout << "Test 4: Check remaining jobs:" << std::endl;
    jobs.list(); // 打印作业列表，检查是否正确删除
    
}
// 测试获取前台作业的 PID
void testGetFgPid() {
    Jobs& jobs = Jobs::getInstance();
    pid_t fgpid = jobs.getFgPid();
    // 还可以添加更多测试逻辑，例如检查是否返回了正确的前台作业 PID 等
}

// 测试根据 PID 查找作业
void testGetJobByPid() {
    Jobs& jobs = Jobs::getInstance();
    std::optional<job_t> jobByPid = jobs.getJobByPid(1234);
    // 还可以添加更多测试逻辑，例如检查是否返回了正确的作业信息等
}

// 测试根据 JID 查找作业
void testGetJobByJid() {
    Jobs& jobs = Jobs::getInstance();
    std::optional<job_t> jobByJid = jobs.getJobByJid(1);
    // 还可以添加更多测试逻辑，例如检查是否返回了正确的作业信息等
}

// 测试根据 PID 查找对应的 JID
void testPid2Jid() {
    Jobs& jobs = Jobs::getInstance();
    std::optional<int> jid = jobs.pid2jid(1234);
    // 还可以添加更多测试逻辑，例如检查是否返回了正确的 JID 等
}

void testMaxjid() {
    Jobs& jobs = Jobs::getInstance();

    // 添加几个作业
    jobs.addJob(1234, FG, "ls -l");
    jobs.addJob(5678, BG, "echo Hello");

    // 测试 maxjid() 函数是否返回正确的最大作业 ID
    int maxJid = jobs.maxjid();
    std::cout << "Max JID: " << maxJid << std::endl;
}

// 测试打印作业列表
void testListJobs() {
    Jobs& jobs = Jobs::getInstance();
    int pid = 1234;
    for (int i = 0; i < 16; i++) 
        jobs.addJob(pid++, BG, "hello");
    pid = 1234;
    for (int i = 0; i < 8; i++) 
        jobs.deleteJob(pid++);

    
    cout << "-----------------------" << endl;
    jobs.list();

    printTestedInfo(__func__);
    // 还可以添加更多测试逻辑，例如检查打印的作业列表是否正确等
}

// 主测试函数
int main() {
    // testAddJob();
    // testDeleteJob();
    testMaxjid();
    // testGetFgPid();
    // testGetJobByPid();
    // testGetJobByJid();
    // testPid2Jid();
    // testListJobs();
    return 0;
}
