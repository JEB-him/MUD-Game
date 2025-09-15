/**
* @file FinalExam.cpp
* @author Xiaotian Wu(Cheongfan)
*/

#include"FinalExam.h"
#include "InputHandler.h"
#include "json.hpp"
#include"View.h"
#include "tools.h"
#include<filesystem>
#include<string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include<sstream>

//不重复随机数组接口
std::vector<int> GenerateDiffNumber(int min, int max, int num) {
	int rnd;
	std::vector<int> diff;
	std::vector<int> tmp;
	for (int i = min; i <= max; i++) {
		tmp.push_back(i);
	}
	srand((unsigned)time(0));
	for (int i = 0; i < num; i++) {
		do {
			rnd = min + rand() % (max - min + 1);
		} while (tmp.at(rnd - min) == -1);
		diff.push_back(rnd);
		tmp.at(rnd - min) = -1;
	}
	return diff;
}

/**
 * @brief 问题类构造函数，通过序号从题库中初始化问题（这里的序号之后会代入一系列不重复随机数，实现随机挑选）
 * @param index 题库中的题目序号
 * @param questions 题库引用
 */
Question::Question():choices(0){ }

Message Question::readQuestions(int index, nlohmann::json& questions) {
	std::stringstream ss_i;
	ss_i << index;
	question = questions[ss_i.str()]["question"];
	answer = questions[ss_i.str()]["answer"];
	is_sci = questions[ss_i.str()]["is_sci"];

	///> 初始化选项
	std::stringstream ss;
	int i = 1;
	ss << "choice" << i ;
	while (questions[ss_i.str()].contains(ss.str())) {
		choices.push_back(questions[ss_i.str()][ss.str()]);
		if (questions[ss_i.str()][ss.str()] == answer)answer_order = i;
		i++;
		ss.str("");
		ss << "choice" << i;
	}

	return { "成功从题库中读取题目",0 };
}

/**
 * @brief 格式化输出问题
 * @param index 该问题在试卷上的序号
 */
Message Question::showQuestion(int index) {
	auto view = View::getInstance();
	std::stringstream ss;
	view->printQuestion("", "================", "white");
	ss << "问题" << index << ": " << question;
	view->printQuestion("", ss.str(), "white");
	std::vector<std::string> ops;
	for (int i = 0; i < choices.size(); i++) {
		ss.str("");
		ss << "选项" << i+1 << ": " << choices[i];
		ops.push_back(ss.str());
	}
	view->printOptions(ops);
	view->printQuestion("", "=======ffffff=========", "white");

	return { "成功输出问题。",0 };
}

std::string Question::getAnswer()const {
	return answer;
}
int Question::getAnswerOrder()const {
	return answer_order;
}
bool Question::getIsSci()const {
	return is_sci;
}

/**
 * @brief 期末考试类构造函数
 */
FinalExam::FinalExam() : num_questions(0), num_right(0), num_wrong(0) {
	exam_paper.resize(10);
}

/**
 * @brief 从题库中随机选择num道题
 * @param num 获取的题目数量
 */
Message FinalExam::selectQuestionsInRandom(int num) {	
	//读取信息
	std::filesystem::path file_path(ROOT_DIR);
	file_path = file_path / ".config/final_exam_questions.json";
	std::ifstream file;
	file.open(file_path);
	if (!file.is_open()) {
		throw std::runtime_error("无法打开期末试题配置文件");
	}
	nlohmann::json questions;
	file >> questions;
	file.close();

	//判断num合法性
	int num_questions = questions["0"]["num_questions"];
	if (num <= 0 || num > num_questions) {
		return { "要求生成的题目数量过多或不合法！",1 };
	}

	//生成[1,num_questions]随机整数数组，长度为num	
	std::vector<int> questions_index = GenerateDiffNumber(1, num_questions, num);

	////读取试题
	for (int i = 0; i < num; i++) {
		if (exam_paper.size() <= i) {
			exam_paper.resize(exam_paper.size() * 10);
		}
		exam_paper[i].readQuestions(questions_index[i], questions);
	}
	return  { "成功随机读取试题",0 };
}

/**
 * @brief 玩家进行期末考试
 * @param protagonist 主角类引用 
 */
Message FinalExam::examing(Protagonist& protagonist) {
	auto view = View::getInstance();
	auto controller = Controller::getInstance();
	std::stringstream ss;

	//每100智力覆盖一道错题。
	float intel_sci = protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::INTEL_SCI];
	float intel_arts = protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::INTEL_ARTS];
	intel_sci = (int)(intel_sci / 100);
	intel_arts = (int)(intel_arts / 100);
	
	//遍历每一道题
	for (int i = 0; i < exam_paper.size(); i++) {
		//打印题目
		exam_paper[i].showQuestion(i+1);
		//获取答案题号
		int answer = exam_paper[i].getAnswerOrder();

		//等待玩家输入答案
		view->printQuestion("", "请作答：", "white");
		int response = controller->input->waitKeyDown() - '0';
		
		view->printQuestion("", std::to_string(response), "white");
		view->printQuestion("", std::to_string(answer), "white");
		//判断正误
		if (response == answer) {
			view->printQuestion("", "回答正确！", "white");
			num_right++;
		}else
		{
			if (exam_paper[i].getIsSci()) {
				if (intel_sci > 0) {
					ss << "你原本想要选" << std::to_string(response) << "，但最后改成了" << std::to_string(answer) << "，回答正确！";
					view->printQuestion("", ss.str(), "white");
					num_right++;
					intel_sci--; ///< 理科覆盖机会-1
				}
				else
				{
					view->printQuestion("", "回答错误！", "white");
					num_wrong++;
				}

			}else{
				if (intel_arts > 0) {
					ss << "你原本想要选" << std::to_string(response) << "，但最后改成了" << std::to_string(answer) << "，回答正确！";
					view->printQuestion("", ss.str(), "white");
					num_right++;
					intel_arts--; ///< 文科覆盖机会-1
				}
				else
				{
					view->printQuestion("", "回答错误！", "white");
					num_wrong++;
				}
			}
		}
	}
	return { "期末考试已完成",0 };
}

/**
 * @brief 输出期末成绩及鼓励词
 */
Message FinalExam::printFinalResult()const {
	auto view = View::getInstance();
	std::stringstream ss;

	//计算最终成绩
	float result = ((float)num_right / (float)(num_right + num_wrong)) * 100.0f;

	//格式化输出期末成绩与激励语
	view->printQuestion("", "================", "white");
	ss << "本次游戏中，您期末考试的最终成绩为：" << result;
	view->printQuestion("", ss.str(), "white");
	if (result >= 60.0f) {
		view->printQuestion("", "恭喜您通过了本学期的期末考试！", "white");
	}else{
		view->printQuestion("", "很遗憾，您本学期挂科了，请再接再厉！", "white");
	}

	return { "期末成绩已打印。",0 };
}