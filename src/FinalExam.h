/**
* @file FinalExam.h
* @author Xiaotian Wu(Cheongfan)
* @note 创建FinalExam对象，然后连续调用selectQuestionsInRandom、examing、printFinalResult三个方法即可。
*/

#pragma once
#include "json.hpp"
#include "tools.h"
#include <Protagonist.h>
#include <string>

/**
* @brief 问题类
* @param question 问题的正文
* @param choices 问题选项的文本（不带序号）
* @param answer 正确答案的文本
* @param answer_order 正确答案的序号
* @param is_sci 是否为理科题
*/
class Question {
public:
	Question(int index, nlohmann::json& questions); ///< 构造函数，通过json文件和json题号初始化问题对象
	~Question() = default;  ///< 默认析构
	Message showQuestion(int index); ///< 格式化输出问题,index为输出的问题序号
	std::string getAnswer()const; ///< 获取答案的文本
	int getAnswerOrder()const; ///< 获取答案的序号
	bool getIsSci()const; ///< 获取该题是否为理科题
private:
	std::string question;
	std::vector<std::string> choices;
	std::string answer;
	int answer_order;
	bool is_sci;
};

/**
* @brief 期末考试类
* @param exam_paper 试卷（问题数组）
* @param num_right 做对的题数
* @param num_wrong 做错的题数
*/
class FinalExam {
public:
	FinalExam(); ///< 构造函数
	~FinalExam() = default; ///< 默认析构
	Message selectQuestionsInRandom(int num); ///< 从题库中随机挑选num道题目
	Message examing(Protagonist& protagonist); ///< 让玩家做所有挑选出来的题
	Message printFinalResult()const; ///< 打印期末考试成绩（结算画面）
private:
	std::vector<Question> exam_paper;
	int num_right;
	int num_wrong;
};
