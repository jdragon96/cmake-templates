#ifndef DB_MODEL_STUDENT_H
#define DB_MODEL_STUDENT_H

#include <mongocxx/client.hpp>
#include <mongocxx/client_session.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <string>
#include <tuple>
#include <vector>

// #include "robot_db/helper/property.hpp"
#include "db/model/Document.h"
#include "db/model/SubDocument.h"
#include "db/resource/MongoManager.hpp"

namespace DB {
namespace Model {
// using namespace Helper;

enum GradeEnum {
	A,
	B,
	C,
	D,
	E
};

class StudentGrade : SubDocument {
		using Key = std::tuple<std::string&, bsoncxx::builder::basic::document&>;

	public:
		//! 수업 명
		std::string Class;

		//! 등급
		GradeEnum Grade;

	public:
		//! 도큐먼트 갱신 시 사용되는 document를 생성한다
		void MakeUpdateDoc(bsoncxx::builder::basic::document& interDoc) {
			using namespace bsoncxx::builder::basic;
			document doc({});
			doc.append(kvp("Class", this->Class));
			doc.append(kvp("Grade", static_cast<int>(this->Grade)));
			interDoc.append(kvp(this->Class, doc));
		}
};

class Student : Document {
	public:
		//! oid
		std::string _id;

		//! 이름
		std::string Name;

		//! 나이
		int Age;

		//! 주소
		std::string Address;

		//! 학생번호
		std::string StudentID;

		//! 성적
		std::vector<StudentGrade> Grades;

	public:
		//! 생성자 - 항상 고정해둘 것
		Student() {
			m_dbName = "RobotLive";
			m_collectionName = "Student";
		}

		//! 소멸자
		~Student() {}

		//! Document::value 객체를 저장한다.
		bool autoParser();

		//! 키를 기준으로 Upsert를 실행한다.
		bool insertOne(Resource::MongoManager::connection& conn, mongocxx::v_noabi::client_session* s = nullptr);
		// bool insertOne(Resource::MongoManager::connection& conn);

		//! 키를 기준으로 특정 학생을 찾는다.
		bool findOne(Resource::MongoManager::connection& conn);

		//! 키를 기준으로 특정 도큐먼트를 지운다.
		bool deleteOne(Resource::MongoManager::connection& conn, mongocxx::v_noabi::client_session* s = nullptr);

		//! transaction으로 delete
		bool transactionExample(Resource::MongoManager::connection& conn);

		//! Bulk update를 위한 document 생성

		//! ...

	private:
		//! Student collection 내 도큐먼트의 unique key롤 생성한다.
		bsoncxx::builder::basic::document MakeKey();

		//! 업데이트 document를 생성한다.
		bsoncxx::builder::basic::document MakeUpdateDoc();
};

}	 // namespace Model
}	 // namespace DB
#endif