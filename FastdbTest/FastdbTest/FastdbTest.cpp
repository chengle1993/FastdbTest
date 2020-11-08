//-< TESTDB.CPP >----------------------------------------------------*--------*
// FastDB                    Version 1.0         (c) 1999  GARRET    *     ?  *
// (Main Memory Database Management System)                          *   /\|  *
//                                                                   *  /  \  *
//                          Created:     10-Dec-98    K.A. Knizhnik  * / [] \ *
//                          Last update: 12-Dec-98    K.A. Knizhnik  * GARRET *
//-------------------------------------------------------------------*--------*
// Sample of database application: supplier/contract/details database
//-------------------------------------------------------------------*--------*

/************************************************************************/
/* 
表（以及对应的类）在每一时刻只能对应于一个数据库。当你打开一个数据库，fastdb向数据库中导入所有在应用中定义的类。
一个类T的游标包含一个类T的实例，用来获得当前的记录。
dbDatabase类控制应用与数据库的交互，进行数据库并发访问的同步，事务处理，内存分配，出错处理...
在一个多线程的应用中，每一个要访问数据库的线程都应该首先与数据库粘附(attach). dbDatabase::attach()方法分配线程指定数据然后把线程与数据库粘附起来。该方法自动由open()方法调用。因此没有理由为打开数据的线程调用attach()方法。当该线程工作完毕，应当调用dbDatabase::detach() 方法。close方法自动调用detach()方法。detach()方法隐含提交当前事务。一个已经分离的线程试图访问数据库时将产生断言错误（assertion failure）。
fastdb可以并行的编译和执行查询，在多处理器系统中显著的提高了性能。但不能并发更新数据库.
在数据库层只使用共享锁和排它锁使得fastdb几乎清除锁开销从而优化无冲突操作的执行速度。但是如果多个应用同时更新数据库的不同部分，则fastdb使用的方法将非常低效。这就是为什么fastdb主要适用于单应用程序数据局访问模式或者多应用读优势(read-dominated)访问模式模型。

FastDB不支持join，所以没有外键的设置，表之间是通过dbReference<T> t来联系的。

FIELD(name)                             指定非索引字段的名字
KEY(name, index_type)                   指定索引字段的名字
RELATION(reference, inverse_reference)  指定类（表）之间的一对一、一对多或者多对多的关系。
METHOD(name)                            为类指定一个方法。该方法必须是无参的实例成员函数，返回bool值、数值、引用或者字符串类型。方法必须在类的所有属性之后指定。

Query类用于两个目的：
1．构造一个查询并绑定查询参数
2．作为已编译的查询的缓存
fastdb提供重载的c++运算符'='和','来构造带参数的查询语句。参数可以在被使用的地方直接指定，消除了在参数占位符和c变量之间的任何映像。

Cursor
游标用来访问选择语句返回的记录。fastdb提供了有类型的游标，也就是说，与具体表相关的游标。fastdb有两种游标：只读游标和用于更新的游标。缺省创建一个只读游标。要创建一个用于更新的游标，必须给构造函数传递一个dbCursorForUpdate参数。
执行一个查询要么通过游标select（dbQuery &q）方法，要么通过select()方法，后者可以迭代表中的所有记录。两个方法都返回中选的记录的数量，并且把当前位置置于第一个记录(如果有的话)。游标可以前滚或者后滚。next(),prev(),first(),last()方法可以用来改变游标的当前位置。如果由于没有（更多）的记录存在而使得操作无法进行，这些方法将返回NULL，并且游标的位置不改变。
一个类T的游标包含一个类T的实例，用来获得当前的记录。这就是为什么表类必须要有一个缺省构造函数(无参数的构造函数)而没有副作用。
当更新记录后，数据库的大小可能会增加。从而虚拟存储器的数据库区域需要进行扩展。该重映射的后果之一就是，该区域的基地址可能发生变化，然后应用程序中保存的所有数据库指针都变得无效。当数据库区域重映象时fastdb自动更新所有打开的游标中的当前记录。因此，当一个数据库更新时，程序员应该通过游标的->方法来访问记录字段，而不应该使用指针变量。
当前选择使用的内存可以通过reset()方法来释放。该方法自动的由select()、dbDatabase::commit()、dbDatabase::rollback()方法以及游标的销毁（destructor）函数调用，因此大多数情况下不需要显式调用reset()方法。

*/
/************************************************************************/


#include "fastdb/fastdb.h"
#include <stdio.h>

USE_FASTDB_NAMESPACE

class Contract;

class Detail {
public:
	char const* name;
	char const* material;
	char const* color;
	real4       weight;

	dbArray< dbReference<Contract> > contracts;

	//所有数据库中使用的类都要定义类型描述符。
	TYPE_DESCRIPTOR((KEY(name, INDEXED | HASHED),
		KEY(material, HASHED),
		KEY(color, HASHED),
		KEY(weight, INDEXED),
		RELATION(contracts, detail)));
};

class Supplier {
public:
	char const* company;
	char const* location;
	bool        foreign;

	dbArray< dbReference<Contract> > contracts;

	TYPE_DESCRIPTOR((KEY(company, INDEXED | HASHED),
		KEY(location, HASHED),
		FIELD(foreign),
		RELATION(contracts, supplier)));
};


class Contract {
public:
	dbDateTime            delivery;
	int4                  quantity;
	db_int8               price;
	dbReference<Detail>   detail;
	dbReference<Supplier> supplier;

	TYPE_DESCRIPTOR((KEY(delivery, HASHED | INDEXED),
		KEY(quantity, INDEXED),
		KEY(price, INDEXED),
		RELATION(detail, contracts),
		RELATION(supplier, contracts)));
};

// 创建数据表,建立C++ 类和数据库表之间的映射。如果在一个应用程序中要使用多个数据库，可以通过宏REGISTER_IN(name, database)在具体的数据库中登记一张表,
// 这个宏的参数database应该是一个指向对象dbDatabase 的指针。
REGISTER(Detail);
REGISTER(Supplier);
REGISTER(Contract);

void input(char const* prompt, char* buf, size_t buf_size)
{
	char* p;
	do {
		printf(prompt);
		*buf = '\0';
		fgets(buf, (int)buf_size, stdin);
		p = buf + strlen(buf);
	} while (p <= buf + 1);

	if (*(p - 1) == '\n') {
		*--p = '\0';
	}
}

int main()
{
	const int maxStrLen = 256;

	dbDatabase db;

	char buf[maxStrLen];
	char name[maxStrLen];
	char company[maxStrLen];
	char material[maxStrLen];
	char address[maxStrLen];

	int d, m, y;
	int i, n;
	int choice;
	int quantity;
	db_int8 price;

	dbDateTime from, till;

	Contract contract;
	Supplier supplier;
	Detail detail;

	dbQuery q1, q2, q3, q4, q6, q9, q10;
	q1 = "exists i:(contracts[i].supplier.company=", company, ")";
	q2 = "name like", name;
	q3 = "supplier.location =", address;
	q4 = between("delivery", from, till), "and price >", price,
		"order by", dbDateTime::ascent("delivery");
	q6 = "price >=", price, "or quantity >=", quantity;
	q9 = "company =", company;
	q10 = "supplier.company =", company, "and detail.name like", name;

	dbCursor<Detail>   details;
	dbCursor<Contract> contracts;
	dbCursor<Supplier> suppliers;
	dbCursor<Contract> updateContracts(dbCursorForUpdate);

	if (db.open(_T("testdb"))) {
		while (true) {
			printf(
				"\n\n    MENU:\n\
				1.  Details shipped by supplier\n\
				2.  Suppliers of the detail\n\
				3.  Contracts from specified city\n\
				4.  Expensive details to be delivered in specified period\n\
				5.  Foreign suppliers\n\
				6.  Important contracts\n\
				7.  New supplier\n\
				8.  New detail\n\
				9.  New contract\n\
				10. Cancel contract\n\
				11. Update contract\n\
				12. Exit\n\n");
			input(">> ", buf, sizeof buf);
			if (sscanf(buf, "%d", &choice) != 1) {
				continue;
			}
			switch (choice) {
			case 1:
				printf("Details shipped by supplier\n");
				input("Supplier company: ", company, sizeof company);
				if (details.select(q1) > 0) {
					printf("Detail\tMaterial\tColor\tWeight\n");
					do {
						printf("%s\t%s\t%s\t%f\n",
							details->name, details->material,
							details->color, details->weight);
					} while (details.next());
				}
				else {
					printf("No details shipped by this supplier\n");
				}
				break;
			case 2:
				printf("Suppliers of the detail\n");
				input("Regular expression for detail name: ", name, sizeof name);
				if (details.select(q2) > 0) {
					printf("Detail\tCompany\tLocation\tPrice\n");
					do {
						n = (int)details->contracts.length();
						for (i = 0; i < n; i++) {
							contracts.at(details->contracts[i]);
							suppliers.at(contracts->supplier);
							printf("%s\t%s\t%s\t" INT8_FORMAT "\n",
								details->name,
								suppliers->company, suppliers->location,
								contracts->price);
						}
					} while (details.next());
				}
				else {
					printf("No such details\n");
				}
				break;
			case 3:
				printf("Contracts from specified city\n");
				input("City: ", address, sizeof address);
				if (contracts.select(q3) > 0) {
					printf("Detail\tCompany\tQuantity\n");
					do {
						printf("%s\t%s\t%d\n",
							details.at(contracts->detail)->name,
							suppliers.at(contracts->supplier)->company,
							contracts->quantity);
					} while (contracts.next());
				}
				else {
					printf("No contracts with companies in this city");
				}
				break;
			case 4:
				printf("Expensive details to be delivered in specified period\n");
				input("Delivered after (DD-MM-YYYY): ", buf, sizeof buf);
				if (sscanf(buf, "%d-%d-%d\n", &d, &m, &y) != 3) {
					printf("Wrong date\n");
					continue;
				}
				from = dbDateTime(y, m, d);
				input("Delivered before (DD-MM-YYYY): ", buf, sizeof buf);
				if (sscanf(buf, "%d-%d-%d\n", &d, &m, &y) != 3) {
					printf("Wrong date\n");
					continue;
				}
				till = dbDateTime(y, m, d);
				input("Minimal contract price: ", buf, sizeof buf);
				if (sscanf(buf, INT8_FORMAT, &price) != 1) {
					printf("Bad value\n");
					continue;
				}
				if (contracts.select(q4) > 0) {
					printf("Detail\tDate\tPrice\n");
					do {
						printf("%s\t%s\t" INT8_FORMAT "\n",
							details.at(contracts->detail)->name,
							contracts->delivery.asString(buf, sizeof buf),
							contracts->price);
					} while (contracts.next());
				}
				else {
					printf("No such contracts\n");
				}
				break;
			case 5:
				printf("Foreign suppliers\n");
				if (suppliers.select("foreign and length(contracts) > 0") > 0){
					printf("Company\tLocation\n");
					do {
						printf("%s\t%s\n", suppliers->company,
							suppliers->location);
					} while (suppliers.next());
				}
				else {
					printf("No such suppliers\n");
				}
				break;
			case 6:
				printf("Important contracts\n");
				input("Minimal contract price: ", buf, sizeof buf);
				if (sscanf(buf, INT8_FORMAT, &price) != 1) {
					printf("Bad value\n");
					continue;
				}
				input("Minimal contract quantity: ", buf, sizeof buf);
				if (sscanf(buf, "%d", &quantity) != 1) {
					printf("Bad value\n");
					continue;
				}
				if (contracts.select(q6) > 0) {
					printf("Company\tPrice\tQuantity\tDelivery\n");
					do {
						printf("%s\t" INT8_FORMAT "\t%d\t%s\n",
							suppliers.at(contracts->supplier)->company,
							contracts->price, contracts->quantity,
							contracts->delivery.asString(buf, sizeof buf,
							"%A %x"));
					} while (contracts.next());
				}
				else {
					printf("No such contracts\n");
				}
				break;
			case 7:
				printf("New supplier\n");
				input("Company name: ", company, sizeof company);
				input("Company location: ", address, sizeof address);
				input("Foreign company (y/n): ", buf, sizeof buf);
				supplier.company = company;
				supplier.location = address;
				supplier.foreign = (*buf == 'y');
				insert(supplier);
				break;
			case 8:
				printf("New detail\n");
				input("Detail name: ", name, sizeof name);
				input("Detail material: ", material, sizeof material);
				input("Detail weight: ", buf, sizeof buf);
				sscanf(buf, "%f", &detail.weight);
				input("Detail color: ", buf, sizeof buf);
				detail.name = name;
				detail.material = material;
				detail.color = buf;
				insert(detail);
				break;
			case 9:
				printf("New contract\n");
				db.lock(); // prevent deadlock
				input("Supplier company: ", company, sizeof company);
				if (suppliers.select(q9) == 0) {
					printf("No such supplier\n");
					continue;
				}
				input("Detail name: ", name, sizeof name);
				if (details.select(q2) == 0) {
					printf("No such detail\n");
					continue;
				}
				else if (details.getNumberOfRecords() != 1) {
					printf("More than one record match this pattern");
					continue;
				}
				input("Contract price: ", buf, sizeof buf);
				sscanf(buf, INT8_FORMAT, &contract.price);
				input("Contract quantity: ", buf, sizeof buf);
				sscanf(buf, "%d", &contract.quantity);
				input("Delivered after (days): ", buf, sizeof buf);
				sscanf(buf, "%d", &d);
				contract.delivery =
					dbDateTime::currentDate() + dbDateTime(24 * d, 0);
				contract.supplier = suppliers.currentId();
				contract.detail = details.currentId();
				insert(contract);
				break;
			case 10:
				printf("Cancel contract\n");
				input("Supplier company: ", company, sizeof company);
				input("Detail name pattern: ", name, sizeof name);
				if (updateContracts.select(q10) == 0) {
					printf("No such contracts\n");
				}
				else {
					updateContracts.removeAllSelected();
					// Just test rollback
					input("Really cancel contract (y/n) ? ", buf, sizeof buf);
					if (*buf != 'y') {
						printf("Not confirmed\n");
						db.rollback();
						continue;
					}
				}
				break;
			case 11:
				printf("Update contract\n");
				input("Supplier company: ", company, sizeof company);
				input("Detail name pattern: ", name, sizeof name);
				if (updateContracts.select(q10) == 0) {
					printf("No such contracts\n");
					break;
				}
				do {
					printf("Contract with company %s for shipping %d details "
						"%s for $" INT8_FORMAT " at %s\n",
						suppliers.at(updateContracts->supplier)->company,
						updateContracts->quantity,
						details.at(updateContracts->detail)->name,
						updateContracts->price,
						updateContracts->delivery.asString(buf, sizeof buf));
					input("Change this contract (y/n) ? ", buf, sizeof buf);
					if (*buf == 'y') {
						input("New contract price: ", buf, sizeof buf);
						sscanf(buf, INT8_FORMAT, &updateContracts->price);
						input("New number of details: ", buf, sizeof buf);
						sscanf(buf, "%d", &updateContracts->quantity);
						updateContracts.update();
					}
				} while (updateContracts.next());
				break;
			case 12:
				input("Do you really want to exit (y/n) ? ", buf, sizeof buf);
				if (*buf == 'y') {
					printf("Close database session\n");
					db.close();
					return EXIT_SUCCESS;
				}
				break;
			default:
				printf("Please choose menu items 1..12\n");
				continue;
			}
			printf("Press any key to continue...\n");
			getchar();
			db.commit();
		}
	}
	else {
		printf("failed to open database\n");
		return EXIT_FAILURE;
	}
}














