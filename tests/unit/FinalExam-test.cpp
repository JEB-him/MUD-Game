#include "catch.hpp"
#include "FinalExam.h"
#include "Protagonist.h"

TEST_CASE("FinalExam function test", "[FinalExam]") {
	Protagonist protagonist;
	FinalExam final_exam;
	SECTION("test") {
		final_exam.selectQuestionsInRandom(10);
		final_exam.examing(protagonist);
		final_exam.printFinalResult();
	}
}