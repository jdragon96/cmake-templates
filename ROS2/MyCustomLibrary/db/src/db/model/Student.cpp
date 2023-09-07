
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/client_session.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/read_concern.hpp>
#include <mongocxx/read_preference.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/write_concern.hpp>
#include <db/model/Student.hpp>
#include <string>
#include <tuple>
#include <vector>

using namespace DB::Model;

bsoncxx::builder::basic::document Student::MakeKey() {
	using namespace bsoncxx::builder::basic;
	document keys({});
	keys.append(kvp("StudentID", this->StudentID));
	return keys;
}

bsoncxx::builder::basic::document Student::MakeUpdateDoc() {
	using namespace bsoncxx::builder::basic;
	document doc({});
	doc.append(kvp("Name", this->Name));
	doc.append(kvp("Age", this->Age));
	doc.append(kvp("Address", this->Address));

	//=> Grade 파싱
	document classDocs({});
	std::cout << "성적 개수 : " << this->Grades.size() << std::endl;
	for (auto it = this->Grades.begin(); it != this->Grades.end(); ++it) {
		it->MakeUpdateDoc(classDocs);
	}
	doc.append(kvp("Classes", classDocs));

	//=> Update용 최종 document 생성
	document resultBody({});
	resultBody.append(kvp("$set", doc));
	return resultBody;
}

bool Student::insertOne(Resource::MongoManager::connection& conn, mongocxx::v_noabi::client_session* s) {
	try {
		//=> Upsert 옵션 활성화
		mongocxx::options::update option;
		option.upsert(true);

		//=> Collection 인스턴스 생성
		auto db = (*conn)[this->m_dbName];
		auto collection = db[this->m_collectionName];
		if (!collection) return false;

		//=> operation
		if (s) {
			collection.update_one(
					*s,
					this->MakeKey().view(),
					this->MakeUpdateDoc().view(),
					option);
		} else {
			collection.update_one(
					this->MakeKey().view(),
					this->MakeUpdateDoc().view(),
					option);
		}
		return true;
	} catch (mongocxx::exception& e) {
		std::cout << e.what() << std::endl;
		return false;
	}
}

bool Student::findOne(Resource::MongoManager::connection& conn) {
	try {
		//=> Collection 인스턴스 생성
		auto collection = (*conn)[this->m_dbName][this->m_collectionName];
		if (!collection) return false;

		//=> Find
		auto find = collection.find_one(this->MakeKey().view());
		if (!find) return false;

		//=> Clear
		this->Grades.clear();

		// json 스트링으로 변환하기
		// std::cout << bsoncxx::to_json(item, bsoncxx::ExtendedJsonMode::k_relaxed) << std::endl;
		//=> 파싱
		bsoncxx::v_noabi::document::value item = find.get();
		this->_id = item["_id"].get_oid().value.to_string();
		std::cout << this->_id << std::endl;
		this->Name = item["Name"].get_string().value.to_string();
		std::cout << this->Name << std::endl;
		this->Address = item["Address"].get_string().value.to_string();
		std::cout << this->Address << std::endl;
		this->StudentID = item["StudentID"].get_string().value.to_string();
		std::cout << this->StudentID << std::endl;
		this->Age = item["Age"].get_int32().value;
		std::cout << this->Age << std::endl;

		//=> Inner document
		auto grade_view = item["Classes"].get_document().value;
		for (auto it = grade_view.begin(); it != grade_view.end(); ++it) {
			std::cout << it->key() << ", " << std::endl;
			StudentGrade grade;
			grade.Class = grade_view[it->key()]["Class"].get_string().value.to_string();
			grade.Grade = static_cast<GradeEnum>(grade_view[it->key()]["Grade"].get_int32().value);
			this->Grades.push_back(grade);
		}

		std::cout << this->Grades.size() << std::endl;
		for (auto a : this->Grades) {
			std::cout << a.Class << ", " << a.Grade << std::endl;
		}

		return true;
	} catch (mongocxx::exception& e) {
		std::cout << e.what() << std::endl;
		return false;
	}
}

bool Student::deleteOne(Resource::MongoManager::connection& conn, mongocxx::v_noabi::client_session* s = nullptr) {
	try {
		//=> Collection 인스턴스 생성
		auto collection = (*conn)[this->m_dbName][this->m_collectionName];
		if (!collection) return false;

		//=> operation
		boost::optional<mongocxx::v_noabi::result::delete_result> result;
		if (s == nullptr) {
			result = collection.delete_one(this->MakeKey().view());
		} else {
			result = collection.delete_one(*s, this->MakeKey().view());
		}

		//=> 삭제 연산 실패
		if (!result) return false;

		//=>삭제된 항목이 없음
		if (result->deleted_count() < 1) return false;

		return true;
	} catch (mongocxx::exception& e) {
		std::cout << e.what() << std::endl;
		return false;
	}
}

bool Student::transactionExample(Resource::MongoManager::connection& conn) {
	using namespace mongocxx;

	//=> 1. 읽기, 쓰기 옵션을 설정한다.
	write_concern wc_majority{};
	wc_majority.acknowledge_level(write_concern::level::k_majority);

	read_concern rc_local{};
	rc_local.acknowledge_level(read_concern::level::k_local);

	read_preference rp_primary{};
	rp_primary.mode(read_preference::read_mode::k_primary);

	client_session::with_transaction_cb callback = [&](client_session* session) {
		// Important::  You must pass the session to the operations.
		this->insertOne(conn, session);
		this->deleteOne(conn, session);
	};

	try {
	} catch (mongocxx::exception& e) {
		std::cout << e.what() << std::endl;
		return false;
	}
}