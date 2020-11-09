#ifndef _FID_H_
#define _FID_H_

//----------------------  Account  -----------------------------
const int FID_ACCOUNT_BASE = 1000;
const int FID_ACCOUNT_ID = FID_ACCOUNT_BASE + 0;      // "AccountID"
const int FID_ACCOUNT_NAME = FID_ACCOUNT_BASE + 1;      // "AccountName"
const int FID_ACCOUNT_PASSWORD = FID_ACCOUNT_BASE + 2;      // "AccountPassword"
const int FID_ACCOUNT_COMPANY_ID = FID_ACCOUNT_BASE + 3;      // "AccountCompanyID"
const int FID_ACCOUNT_TOKEN = FID_ACCOUNT_BASE + 4;      // "AccountToken"
const int FID_ACCOUNT_DEPARTMENT = FID_ACCOUNT_BASE + 5;      // "AccountDepartment": BCO - 信用，BNC - 利率
const int FID_ACCOUNT_COMPANY_NAME = FID_ACCOUNT_BASE + 6;      // "AccountCompanyName"
const int FID_ACCOUNT_ROLE = FID_ACCOUNT_BASE + 7;      // "AccountRole": 例如：BCO-DI, BNC-BROKER
const int FID_ACCOUNT_PERMISSION = FID_ACCOUNT_BASE + 8;      // "AccountPermission" 用户拥有的所有权限，以','为间隔
const int FID_ACCOUNT_CODE = FID_ACCOUNT_BASE + 9;      // "AccountCode"
const int FID_ACCOUNT_TELEPHONE = FID_ACCOUNT_BASE + 10;     // "AccountTelephone"
const int FID_ACCOUNT_PHONE = FID_ACCOUNT_BASE + 11;     // "AccountPhone"
const int FID_ACCOUNT_EMAIL = FID_ACCOUNT_BASE + 12;     // "AccountEmail"
const int FID_ACCOUNT_RM = FID_ACCOUNT_BASE + 13;     // "AccountRM"
const int FID_ACCOUNT_MSN = FID_ACCOUNT_BASE + 14;     // "AccountMSN"
const int FID_ACCOUNT_QQ = FID_ACCOUNT_BASE + 15;     // "AccountQQ"
const int FID_ACCOUNT_ADDRESS = FID_ACCOUNT_BASE + 16;     // "AccountAddress"
const int FID_ACCOUNT_PASSWORD_ORG = FID_ACCOUNT_BASE + 17;      // "Original AccountPassword"
const int FID_ACCOUNT_DEPT = FID_ACCOUNT_BASE + 18;	// idb_account.idb_account->dept column 总部:0，分部:1
const int FID_ACCOUNT_PASSWORD_SIMPLE = FID_ACCOUNT_BASE + 19;		// md5一次
const int FID_ACCOUNT_PASSWORD_VALID_TIME = FID_ACCOUNT_BASE + 20;	// 密码到期日
const int FID_ACCOUNT_DISPLAY_NAME = FID_ACCOUNT_BASE + 21;	// dispaly_name

//----------------------  Login Info ----------------------------- 
const int FID_LOGIN_BASE = 1100;
const int FID_LOGIN_RET_CODE = FID_LOGIN_BASE + 1;         // "LoginRetCode":登录返回码 0 - 成功， 其他 - 错误码
const int FID_LOGIN_RET_MSG = FID_LOGIN_BASE + 2;         // "LoginRetMsg"登录返回信息
const int FID_LOGIN_TOKEN_VERIFY_RET = FID_LOGIN_BASE + 3;         // "LoginTokenVerifyRet"Token验证返回信息 true - 成功,  false - 失败
const int FID_CLIENT_VERSION = FID_LOGIN_BASE + 4;			// "客户端版本号"
const int FID_CLIENT_VERSION_NEW = FID_LOGIN_BASE + 5;		// "客户端版本号" 1.8.4版本开始使用

//----------------------  System Info ----------------------------
const int FID_SYSTEM_BASE = 1200;
const int FID_SERVER_TIME = FID_SYSTEM_BASE + 1;	     //"ServerTime": 服务器时间
const int FID_MAC_ADDRESS = FID_SYSTEM_BASE + 2;
const int FID_IPV4_ADDRESS = FID_SYSTEM_BASE + 3;
const int FID_USER_INFO_USER_NAME = FID_SYSTEM_BASE + 4;
const int FID_USER_INFO_TYPE_ATTRIBUTE = FID_SYSTEM_BASE + 5;
const int FID_USER_INFO_SOFTWARE_NAME = FID_SYSTEM_BASE + 6;
const int FID_USER_INFO_SOFTWARE_VERSION = FID_SYSTEM_BASE + 7;
const int FID_USER_INFO_LANGUAGE = FID_SYSTEM_BASE + 8;
const int FID_USER_INFO_BITWIDTH = FID_SYSTEM_BASE + 9;
const int FID_USER_INFO_MAC_ADDRESS = FID_SYSTEM_BASE + 10;
const int FID_USER_INFO_IP_ADDRESS = FID_SYSTEM_BASE + 11;
const int FID_USER_LOCATION = FID_SYSTEM_BASE + 12;
const int FID_MSG_TIME = FID_SYSTEM_BASE + 13;
const int FID_SERVICE_STATUS = FID_SYSTEM_BASE + 14;
const int FID_MSG_RESPONSE_TIME = FID_SYSTEM_BASE + 15;

//----------------------  Public --------------------------------
const int FID_BOND_PUBLIC_BASE = 2000;
const int FID_BOND_KEY = FID_BOND_PUBLIC_BASE + 0;   // "BondKey":Bond Key
const int FID_BOND_CODE = FID_BOND_PUBLIC_BASE + 1;   // "BondCode":Bond Code
const int FID_BOND_TYPE = FID_BOND_PUBLIC_BASE + 2;   // "BondType":债券类型【短融，中票...】, 【国债，央票...】
const int FID_BOND_SHORT_NAME = FID_BOND_PUBLIC_BASE + 3;   // "BondShortName":债券简称，比如：10国开09 
const int FID_BOND_CATEGORY = FID_BOND_PUBLIC_BASE + 4;   // "BondCategory":债券种类，BCO- 信用，  BNC-利率
const int FID_PRICE = FID_BOND_PUBLIC_BASE + 5;   // "Price":价格
const int FID_QUOTE_TYPE = FID_BOND_PUBLIC_BASE + 6;   // "QuoteType ":报价类型: 收益率/净价/全价/利差
const int FID_YIELD = FID_BOND_PUBLIC_BASE + 7;   // "Yield":收益率
const int FID_YIELD_TYPE = FID_BOND_PUBLIC_BASE + 8;   // "YieldType":收益率类型0-行权收益率，1-到期收益率
const int FID_MATURITY = FID_BOND_PUBLIC_BASE + 9;   // "Maturity":到期日, 比如：20150329
const int FID_RATE_TYPE = FID_BOND_PUBLIC_BASE + 10;  // "RateType":利率类型，【Shibor，Depo，固息】
const int FID_OPTION_TYPE = FID_BOND_PUBLIC_BASE + 11;  // "OptionType":含权类型, 比如 PUT, CAL
const int FID_CLEAN_PRICE = FID_BOND_PUBLIC_BASE + 12;  // "CleanPrice":净价，103.0941
const int FID_DIRTY_PRICE = FID_BOND_PUBLIC_BASE + 13;  // "DirtyPrice":全价，108.0941
const int FID_SPREAD = FID_BOND_PUBLIC_BASE + 14;  // "Spread":利差
const int FID_ISSUER_RATING = FID_BOND_PUBLIC_BASE + 15;  // "IssuerRating":债券主体评级, 比如AAA, AA, A+...
const int FID_LISTED_MARKET = FID_BOND_PUBLIC_BASE + 16;  // "ListedMarket":交易所，【上交所，深交所，银行间】，CIB - 银行间，SSE-上交所，SZE-深交所
const int FID_DEL_FLAG = FID_BOND_PUBLIC_BASE + 17;  // "DelFlag":删除标记，1-正常，2-已删除
const int FID_INSTITUTION_NAME = FID_BOND_PUBLIC_BASE + 18;  // "InstitutionName":机构名称
const int FID_INSTITUTION_ID = FID_BOND_PUBLIC_BASE + 19;  // "InstitutionID":机构ID
const int FID_TRADER_ID = FID_BOND_PUBLIC_BASE + 20;  // "TraderID":交易员ID
const int FID_TRADER_CODE = FID_BOND_PUBLIC_BASE + 21;  // "TraderCode":交易员Code, 银行交易员，比如：“陈锋”
const int FID_VOLUME = FID_BOND_PUBLIC_BASE + 22;  // "Volume":量
const int FID_SETTLEMENT_TYPE = FID_BOND_PUBLIC_BASE + 23;  // "SettlementType":结算类型，T+0,T+1
const int FID_DELIVERY_DATE = FID_BOND_PUBLIC_BASE + 24;  // "DeliveryDate":交割日
const int FID_GUARENTEED = FID_BOND_PUBLIC_BASE + 25;  // "Guarenteed":有无担保
const int FID_START_DATE = FID_BOND_PUBLIC_BASE + 26;  // "StartDate":起始日期
const int FID_END_DATE = FID_BOND_PUBLIC_BASE + 27;  // "EndDate":结束日期
const int FID_SHOW_ALL = FID_BOND_PUBLIC_BASE + 28;  // "ShowAll":显示所有
const int FID_INTELLIGENT_SORTING = FID_BOND_PUBLIC_BASE + 29;  // "IntelligentSorting":智能排序, 排序字段，选中“智能排序”后，sortField = “goodsLevel+goodsTerm”
const int FID_SORT_SEQUENCE = FID_BOND_PUBLIC_BASE + 30;  // "SortSequence":顺序/倒序, 排序顺序，1-顺序， 0-倒序
const int FID_FILTER_QUOTE = FID_BOND_PUBLIC_BASE + 31;  // "FilterQuote":是否过滤对价, 最优报价里面，选中“对价”checkbox
const int FID_BONDGROUP_ID = FID_BOND_PUBLIC_BASE + 32;  // "BondGroupId":债券分组ID
const int FID_BROKER_ID = FID_BOND_PUBLIC_BASE + 33;  // "BrokerID":经纪人ID
const int FID_ON_THE_RUN = FID_BOND_PUBLIC_BASE + 34;  // "OnTheRun":新上市, 最近5天上市的，即Listed_Dat =  [Date - 4, Date]
const int FID_MARKET_TYPE = FID_BOND_PUBLIC_BASE + 35;  // "MarketType":市场类型, 即一级市场或二级市场，1-意向  2-二级
const int FID_PLEDGED = FID_BOND_PUBLIC_BASE + 36;  // "Pledged":是否可质押，Y-可质押，N-不可质押
const int FID_ACROSS_MARKET = FID_BOND_PUBLIC_BASE + 37;  // "AcrossMarket":是否夸市场，Y-跨市场，N-不跨市场
const int FID_ENTERPRISE_TYPE = FID_BOND_PUBLIC_BASE + 38;  // "EnterpriseType":企业类型, 包括【国企，非国企】SOE-国企，NSO-非国企
const int FID_POLICY_BANK_TYPE = FID_BOND_PUBLIC_BASE + 39;  // "PolicyBankType":国开-Short_Name包含‘国开’，口行-Short_Name包含‘进出’，农发-Short_Name包含‘农发’
const int FID_KEYWORDS = FID_BOND_PUBLIC_BASE + 40;  // "Keywords":搜索关键字
const int FID_MUNICIPAL = FID_BOND_PUBLIC_BASE + 41;  // "Municipal":是否城投债, 是否城投债，Y-城投债，N-非城投债
const int FID_INDEX = FID_BOND_PUBLIC_BASE + 42;  // "Index":索引, 比如翻页索引
const int FID_ROW_COUNT = FID_BOND_PUBLIC_BASE + 43;  // "RowCount":行数
const int FID_BROKER_NAME = FID_BOND_PUBLIC_BASE + 44;  // "BrokerName":经纪人姓名
const int FID_PAGE_INFOR = FID_BOND_PUBLIC_BASE + 45;  // "PageInfor":翻页信息
const int FID_COUPON_RATE = FID_BOND_PUBLIC_BASE + 46;  // "CouponRate":票面利率
const int FID_MODIFY_TIME = FID_BOND_PUBLIC_BASE + 47;  // "ModifyTime":修改时间
const int FID_MATURITY_RANGE_START = FID_BOND_PUBLIC_BASE + 48;  // "MaturityRangeStart":到期日范围的起始日
const int FID_MATURITY_RANGE_END = FID_BOND_PUBLIC_BASE + 49;  // "MaturityRangeEnd":到期日范围的终止日
const int FID_MATURITY_DATE_TYPE = FID_BOND_PUBLIC_BASE + 50;  // "MaturityDateType":到期日是周末还是节假日还是正常工作日
const int FID_LOGIN_ACCOUNT = FID_BOND_PUBLIC_BASE + 51;  // "LoginAccount":登录的Broker ID
const int FID_BOND_KEY_LISTED_MARKET = FID_BOND_PUBLIC_BASE + 52;  // "BondKeyListedMarket":“键值+市场” 唯一标示一个债券
const int FID_PIN_YIN = FID_BOND_PUBLIC_BASE + 53;  // "PinYin":拼音
const int FID_PIN_YIN_FULL = FID_BOND_PUBLIC_BASE + 54;  // "PinYinFull":拼音全称
const int FID_INSTITUTION_CODE = FID_BOND_PUBLIC_BASE + 55;  // "InstitutionCode":机构代码
const int FID_DESCRIPTION = FID_BOND_PUBLIC_BASE + 56;  // "Description":描述,如报价描述，成交描述
const int FID_INSTITUTION_FULL_NAME = FID_BOND_PUBLIC_BASE + 57;  // "InstitutionFullName":机构全称
const int FID_TRADER_NAME = FID_BOND_PUBLIC_BASE + 59;  // "TraderName":交易员名称
const int FID_USER_NAME = FID_BOND_PUBLIC_BASE + 60;  // "UserName":用户名
const int FID_LOGIN_NAME = FID_BOND_PUBLIC_BASE + 61;  // "LoginName":登陆名
const int FID_FAN_DIAN = FID_BOND_PUBLIC_BASE + 62;  // "FanDian":返点数值, 比如返0.12
const int FID_FAN_DIAN_FLAG = FID_BOND_PUBLIC_BASE + 63;  // "FanDianFlag":是否返点
const int FID_DATE_TYPE = FID_BOND_PUBLIC_BASE + 64;  // "DateType":时间类型:成交日/交割日/交易日【"createTime" , "settlementDate", "tradedDate"】
const int FID_PAGE_INDEX = FID_BOND_PUBLIC_BASE + 65;  // "PageIndex":页码，Table的第几页
const int FID_PAGE_SIZE = FID_BOND_PUBLIC_BASE + 66;  // "PageSize":每页显示多少条
const int FID_PAGE_COUNT = FID_BOND_PUBLIC_BASE + 67;  // "PageCount":总页数，总共有多少页
const int FID_TOTAL_RECORDS = FID_BOND_PUBLIC_BASE + 68;  // "TotalRecords":总条数，总共有多少条记录
const int FID_USING_PAGINATION = FID_BOND_PUBLIC_BASE + 69;  // "UsingPagination":是否分页显示
const int FID_HAS_PREVIOUS = FID_BOND_PUBLIC_BASE + 70;  // "HasPrevious":前一页是否能用
const int FID_HAS_NEXT = FID_BOND_PUBLIC_BASE + 71;  // "HasNext":后一页是否能用
const int FID_FIRST = FID_BOND_PUBLIC_BASE + 73;  // "FIRST":常量，值为1，用来表示第一页
const int FID_DEFAULT_PAGE_SIZE = FID_BOND_PUBLIC_BASE + 74;  // "DefaultPageSize":常量，值为10，用来表示默认页大小
const int FID_RETURN_CODE = FID_BOND_PUBLIC_BASE + 75;  // "ReturnCode":0: success, 1: failed
const int FID_ERROR_MESSAGE = FID_BOND_PUBLIC_BASE + 76;  // "ErrorMessage":错误消息
const int FID_CREATE_TIME = FID_BOND_PUBLIC_BASE + 77;  // "CreateTime":创建时间
const int FID_LISTED_DATE = FID_BOND_PUBLIC_BASE + 78;  // "ListedDate": 上市日
const int FID_ID = FID_BOND_PUBLIC_BASE + 79;  // "ID":产品ID
const int FID_UNDER_WRITER_NAME = FID_BOND_PUBLIC_BASE + 81;  // "UnderWriterName":主承销商
const int FID_ISSUE_AMOUNT = FID_BOND_PUBLIC_BASE + 82;  // "IssueAmount":发行量
const int FID_BOND_SUBTYPE = FID_BOND_PUBLIC_BASE + 83;  // "BondSubType":债券分类
const int FID_TIME_TO_MATURITY = FID_BOND_PUBLIC_BASE + 84;  // "TimeToMaturity":剩余期限，如7.5Y
const int FID_PRICE_MAP = FID_BOND_PUBLIC_BASE + 85;  // "PriceMap":价格机构体
const int FID_CDC_VALUATIONS_YIELD = FID_BOND_PUBLIC_BASE + 86;  // "CDCValuationsYield":中债估值收益率
const int FID_CDC_VALUATIONS_CLEAN_PRICE = FID_BOND_PUBLIC_BASE + 87;  // "CDCValuationsCleanPrice":中债估值净价
const int FID_ISSUER_RATING_NPY = FID_BOND_PUBLIC_BASE + 88;  // "IssuerRatingNPY":主体评级（非鹏元）
const int FID_BOND_RATING = FID_BOND_PUBLIC_BASE + 89;  // "BondRating":债券评级
const int FID_BOND_RATING_NPY = FID_BOND_PUBLIC_BASE + 90;  // "BondRatingNPY":债券评级（非鹏元）
const int FID_BID_PRICE_MAP = FID_BOND_PUBLIC_BASE + 91;  // "BidPriceMap": BID Price结构体
const int FID_OFR_PRICE_MAP = FID_BOND_PUBLIC_BASE + 92;  // "OfrPriceMap": OFR Price结构体
const int FID_INSTITUTION_TRADER_NAMES = FID_BOND_PUBLIC_BASE + 93;  // "InstitutionTraderNames":机构(交易员)，比如：农行北京（徐明）"
const int FID_RATING_DATE = FID_BOND_PUBLIC_BASE + 94;  // "RatingDate":评级日期，比如20140619
const int FID_ISSUER_CODE = FID_BOND_PUBLIC_BASE + 95;  // "IssuerCode":发行机构代码, Z000506
const int FID_NEXT_COUPON_DATE = FID_BOND_PUBLIC_BASE + 96;  // "NextCouponDate":下一个付息日，比如20140619    
const int FID_OPTION_DATE = FID_BOND_PUBLIC_BASE + 97;  // "OptionDate":行权日，比如20140619    
const int FID_UNDERWRITER_CODE = FID_BOND_PUBLIC_BASE + 98;  // "UnderwriterCode":承销机构代码    
const int FID_ISSUER_RATING_INSTITUTION_CODE = FID_BOND_PUBLIC_BASE + 99;  // "IssuerRatingInstitutionCode":主体评级机构代码    
const int FID_BOND_RATING_INSTITUTION_CODE = FID_BOND_PUBLIC_BASE + 100; // "BondRatingInstitutionCode":债项评级机构代码
const int FID_COUPON_RATE_SPREAD_BY_PERCENT = FID_BOND_PUBLIC_BASE + 101; // "CouponRateSpreadByPercent":利差的%形式
const int FID_TIME_TO_MATURITY_WITH_OPTION_TERM = FID_BOND_PUBLIC_BASE + 102; // "TimeToMaturityWithOptionTerm":比如 360 + 300，表示360D + 300D
const int FID_REDEMPTION_NO_DESCRIPTION = FID_BOND_PUBLIC_BASE + 103; // "RedemptionNODescription":比如“提前还本”，“到期一次还本”
const int FID_BOND_WARRANTER = FID_BOND_PUBLIC_BASE + 104; // warranter
const int FID_SORTING_DATE = FID_BOND_PUBLIC_BASE + 105; // "SortingDate":按到期日还是行权日排序
const int FID_INSTITUTION_TYPE = FID_BOND_PUBLIC_BASE + 106; // "InstitutionType":【银行，保险公司，债券公司，其他】
const int FID_CAPITAL_TYPE = FID_BOND_PUBLIC_BASE + 107; // "CapitalType":【中资,外资，合资，其他】
const int FID_INTERNAL_INSTITUTION_TYPE = FID_BOND_PUBLIC_BASE + 108; // "InternalInstitutionType":【是，否】内部银行
const int FID_INSTITUTION_CREDIT_TYPE = FID_BOND_PUBLIC_BASE + 109; // "InstitutionCreditType": 银行属性，即好名字和坏名字
const int FID_INSTITUTION_CITY_CODE = FID_BOND_PUBLIC_BASE + 110; // "InstitutionCityCode": 机构所在城市代码
const int FID_INSTITUTION_CITY_NAME = FID_BOND_PUBLIC_BASE + 111; // "InstitutionCityName": 机构所在城市名称
const int FID_INSTITUTION_NAME_EN = FID_BOND_PUBLIC_BASE + 112; // "InstitutionNameEN": 机构英文简称
const int FID_DICTIONARY_CODE = FID_BOND_PUBLIC_BASE + 113; // "DictionaryCode": 查询字典代码
const int FID_DICTIONARY_CONTENT = FID_BOND_PUBLIC_BASE + 114; // "DictionaryContent": 查询字典代码对应的内容
const int FID_DICTIONARY_GROUP_NAME = FID_BOND_PUBLIC_BASE + 115; // "DictionaryGroupName": 查询字典代码所属的组
const int FID_TRADER_FCODE = FID_BOND_PUBLIC_BASE + 116; // "TraderFCode": 交易员F代码
const int FID_TRADABLE_BOND_CATEGORY = FID_BOND_PUBLIC_BASE + 117; // "TradableBondCategory": 交易员可交易债券种类，利率或信用，比如‘BCO’，‘BCO,BNC’
const int FID_CUSTOMER_ID = FID_BOND_PUBLIC_BASE + 118; // "CustomerID": ?
const int FID_CDC_VALUATIONS_DATE = FID_BOND_PUBLIC_BASE + 119; // "CDCValuationsDate": String    中债估值日期
const int FID_MATURITY_TERM = FID_BOND_PUBLIC_BASE + 120; // "MaturityTerm": 剩余期限的数字部分，比如3.5Y,则 MaturityTerm = 3.5
const int FID_TERM_UNIT = FID_BOND_PUBLIC_BASE + 121; // "TermUnit": 剩余期限的单位部分，比如3.5Y,则 TermUnit = Y
const int FID_GUARANTEE_INSTITUTE_CODE = FID_BOND_PUBLIC_BASE + 122; // "GuaranteeInstituteCode": 担保机构代码，Null表示数据不完整，或者没有担保机构
const int FID_BID_TRADER_IS_VIP = FID_BOND_PUBLIC_BASE + 123; // "BidTraderIsVIP": 买方交易员是否是VIP
const int FID_OFR_TRADER_IS_VIP = FID_BOND_PUBLIC_BASE + 124; // "OfrTraderIsVIP": 卖方交易员是否是VIP
const int FID_BID_PRICE = FID_BOND_PUBLIC_BASE + 125; // "BidPrice": Bid价格
const int FID_BID_FAN_DIAN = FID_BOND_PUBLIC_BASE + 126; // "BidFanDian": Bid 返点数
const int FID_BID_FAN_DIAN_FLAG = FID_BOND_PUBLIC_BASE + 127; // "BidFanDianFlag": Bid 返点标识
const int FID_BID_INTERNAL_FLAG = FID_BOND_PUBLIC_BASE + 128; // "BidInternalFlag": Bid 内部标识
const int FID_BID_DESCRIPTION = FID_BOND_PUBLIC_BASE + 129; // "BidDescription": Bid 报价描述
const int FID_BID_BARGAIN_FLAG = FID_BOND_PUBLIC_BASE + 130; // "BidBargainFlag": Bid 议价标识
const int FID_BID_OCO_FLAG = FID_BOND_PUBLIC_BASE + 131; // "BidOCOFlag": Bid OCO标识, 0-代表不显示，1-显示oco  2-oco打包
const int FID_OFR_PRICE = FID_BOND_PUBLIC_BASE + 132; // "OfrPrice": Ofr 价格
const int FID_OFR_FAN_DIAN = FID_BOND_PUBLIC_BASE + 133; // "OfrFanDian": Ofr 返点数
const int FID_OFR_FAN_DIAN_FLAG = FID_BOND_PUBLIC_BASE + 134; // "OfrFanDianFlag": Ofr 返点标识
const int FID_OFR_INTERNAL_FLAG = FID_BOND_PUBLIC_BASE + 135; // "OfrInternalFlag": Ofr 内部标识
const int FID_OFR_DESCRIPTION = FID_BOND_PUBLIC_BASE + 136; // "OfrDescription": Ofr 报价描述
const int FID_OFR_BARGAIN_FLAG = FID_BOND_PUBLIC_BASE + 137; // "OfrBargainFlag": Ofr 议价标识
const int FID_OFR_OCO_FLAG = FID_BOND_PUBLIC_BASE + 138; // "OfrOCOFlag": Ofr OCO标识, 0-代表不显示，1-显示oco  2-oco打包
const int FID_BID_BAD_FLAG = FID_BOND_PUBLIC_BASE + 139; // "BidBadFlag": Bid方是Bad
const int FID_OFR_BAD_FLAG = FID_BOND_PUBLIC_BASE + 140; // "OfrBadFlag": Ofr方是Bad
const int FID_BID_IS_OWN_PRICE = FID_BOND_PUBLIC_BASE + 141; // "BidIsOwnPrice": Bid我的报价
const int FID_OFR_IS_OWN_PRICE = FID_BOND_PUBLIC_BASE + 142; // "OfrIsOwnPrice": Ofr我的报价
const int FID_HAS_OPTION = FID_BOND_PUBLIC_BASE + 143; // "HasOption" : 是否含权
const int FID_BID_VOLUME = FID_BOND_PUBLIC_BASE + 144; // "BidVolume": BID量
const int FID_OFR_VOLUME = FID_BOND_PUBLIC_BASE + 145; // "OfrVolume": OFR量
const int FID_BID_QUOTE_ID = FID_BOND_PUBLIC_BASE + 146; // "BidQuoteID": Bid报价ID
const int FID_OFR_QUOTE_ID = FID_BOND_PUBLIC_BASE + 147; // "OfrQuoteID": Ofr报价ID
const int FID_BID_PRICE_STRING = FID_BOND_PUBLIC_BASE + 148; // "BidPriceString"
const int FID_BID_FAN_DIAN_STRING = FID_BOND_PUBLIC_BASE + 149; // "BidFanDianString"
const int FID_OFR_PRICE_STRING = FID_BOND_PUBLIC_BASE + 150; // "OfrPriceString"
const int FID_OFR_FAN_DIAN_STRING = FID_BOND_PUBLIC_BASE + 151; // "OfrFanDianString"
const int FID_PRICE_STRING = FID_BOND_PUBLIC_BASE + 152; // "PriceString"
const int FID_FAN_DIAN_STRING = FID_BOND_PUBLIC_BASE + 153; // "FanDianString"
const int FID_BID_OFR_SPREAD = FID_BOND_PUBLIC_BASE + 154; // "BidOfrSpread"
const int FID_CDC_OFFSET = FID_BOND_PUBLIC_BASE + 155; // "CDCOffset"
const int FID_TRADER_BARGAIN = FID_BOND_PUBLIC_BASE + 156; // "Trader: Bargain"
const int FID_ACROSS_MARKET_BOND_CODES = FID_BOND_PUBLIC_BASE + 157; // "AcrossMarketBondCodes": 跨市场所有Bond Codes
const int FID_BOND_IS_EXPIRED = FID_BOND_PUBLIC_BASE + 160; // "IsExpired"
const int FID_MATURITY_HOLIDAYS = FID_BOND_PUBLIC_BASE + 161; // "maturity_holidays"
const int FID_MATURITY_HOLIDAYS_SSE = FID_BOND_PUBLIC_BASE + 162; // "maturity_holidays_sse"
const int FID_MATURITY_HOLIDAYS_SZE = FID_BOND_PUBLIC_BASE + 163; // "maturity_holidays_sze"
const int FID_INSTITUTION_QB_ID = FID_BOND_PUBLIC_BASE + 164;
const int FID_TRADER_QB_ID = FID_BOND_PUBLIC_BASE + 165;
const int FID_AUCTION_DATE_START = FID_BOND_PUBLIC_BASE + 166; // 招标日起始日期
const int FID_AUCTION_DATE_END = FID_BOND_PUBLIC_BASE + 167; // 招标日结束日期
const int FID_ISSUER_CITY = FID_BOND_PUBLIC_BASE + 168;	// 发行机构所在城市
const int FID_ISSUER_CITY_LEVEL = FID_BOND_PUBLIC_BASE + 169; // 发行机构城市级别
const int FID_MUNICIPAL_CODE = FID_BOND_PUBLIC_BASE + 170;
const int FID_DELISTED_DATE = FID_BOND_PUBLIC_BASE + 171;  // "DelistedDate": 下市日
const int FID_BID_FLAG_URGENT = FID_BOND_PUBLIC_BASE + 172;  // bid_flag_urgent false:普通，true:紧急
const int FID_OFR_FLAG_URGENT = FID_BOND_PUBLIC_BASE + 173;  // ofr_flag_urgent false:普通，true:紧急
const int FID_LOCAL_GOV_TYPE_CLIENT = FID_BOND_PUBLIC_BASE + 174; // local_gov_type_client，地方债类型，复制行情时使用

const int FID_ACTION_TYPE = FID_BOND_PUBLIC_BASE + 200;  // "ActionType":推送消息类型，Add，Update, Delete
const int FID_LIST = FID_BOND_PUBLIC_BASE + 201;  // "List":用来标识List
const int FID_MAP = FID_BOND_PUBLIC_BASE + 202;  // "Map":用来标识Map
const int FID_ZIP_DATA = FID_BOND_PUBLIC_BASE + 203;  // "ZipData":用来标识压缩数据
const int FID_ZIP_LEN = FID_BOND_PUBLIC_BASE + 204;  // "ZipLen":用来标识压缩数据长度
const int FID_UNZIP_LEN = FID_BOND_PUBLIC_BASE + 205;  // "ZipLen":用来标识压缩数据长度
const int FID_LIST2 = FID_BOND_PUBLIC_BASE + 206;	// "List":用来标识List，一个protobuf中可能有两个repeat

const int FID_CONDITIONS = FID_BOND_PUBLIC_BASE + 210;  // "Conditions":条件, 比如 “FID1 = value1 and FID2 < value2”
const int FID_SORT_BY = FID_BOND_PUBLIC_BASE + 211;  // "SortBy":排序, "FID1 asc, FID2 desc"
const int FID_RANGE = FID_BOND_PUBLIC_BASE + 212;  // "Range":范围, "1, 1000"

const int FID_TOTAL_PACKAGES = FID_BOND_PUBLIC_BASE + 220;  // "TotalPackages":总包数
const int FID_EXECUTIME = FID_BOND_PUBLIC_BASE + 221;  // "Executime":执行时间
const int FID_RATE_INDEX = FID_BOND_PUBLIC_BASE + 222;	 // "RateIndex":用来存储“DEPO”，“REPO”等
const int FID_GUARANTEE_TYPE = FID_BOND_PUBLIC_BASE + 223;  // "GuaranteeType":担保方式
const int FID_ISSUE_INSTITUTION = FID_BOND_PUBLIC_BASE + 224;  // "IssueInstitution":发行机构（发行人）
const int FID_RATING_INSTITUTION = FID_BOND_PUBLIC_BASE + 225;  // "RatingInstitution":债券评级机构
const int FID_ISSUE_INSTITUTION_SHORT = FID_BOND_PUBLIC_BASE + 226; // "IssueInstitution":发行机构简称（发行人简称）

const int FID_ISSUER_RATING_FILTER = FID_BOND_PUBLIC_BASE + 230;  // "IssuerRating":债券主体评级, 用于过滤，比如AAA, AA, 其他...
const int FID_TIME_TO_MATURITY_REAL = FID_BOND_PUBLIC_BASE + 231;  // "TimeToMaturityReal":剩余期限，浮点数，单位year
const int FID_EXPORTED_TEXT = FID_BOND_PUBLIC_BASE + 232;  // "ExportedText" : 各种导出文本

const int FID_BOND_TYPE_ID = FID_BOND_PUBLIC_BASE + 233;  // "BondTypeId": 债券类型id
const int FID_ISSUE_INSTITUTION_PINYIN = FID_BOND_PUBLIC_BASE + 234;  // 发行人的pinyin
const int FID_ISSUE_INSTITUTION_PINYINFULL = FID_BOND_PUBLIC_BASE + 235;  // 发行人的pinyinfull
const int FID_ISSUER_RATING_ORDER = FID_BOND_PUBLIC_BASE + 240;
const int FID_ISSUER_RATING_NPY_ORDER = FID_BOND_PUBLIC_BASE + 241;
const int FID_BOND_RATING_ORDER = FID_BOND_PUBLIC_BASE + 242;
const int FID_BOND_RATING_NPY_ORDER = FID_BOND_PUBLIC_BASE + 243;

const int FID_BOND_TYPE_STATUS = FID_BOND_PUBLIC_BASE + 244;
const int FID_BOND_TYPE_MODIFY_TIME = FID_BOND_PUBLIC_BASE + 245;
const int FID_FILTER_MATURITY = FID_BOND_PUBLIC_BASE + 246;   // 过滤字段，含权债返回option_date，非含权债返回maturity_date
const int FID_OPERATION_TYPE = FID_BOND_PUBLIC_BASE + 247;   // 增量表操作类型：Add Update Delete
const int FID_OPTION_TYPE_CLIENT = FID_BOND_PUBLIC_BASE + 248;   // 期权性质：几年期权到期+几年债券到期
const int FID_MAX_ISSUE_YEAR = FID_BOND_PUBLIC_BASE + 249;	  //bond最大发行年份
const int FID_MIN_ISSUE_YEAR = FID_BOND_PUBLIC_BASE + 250;	  //bond最小发行年份
const int FID_ISSUE_YEAR = FID_BOND_PUBLIC_BASE + 251;   //bond发行年份
const int FID_SW_SECTOR_LEVEL = FID_BOND_PUBLIC_BASE + 252;   //行业级别
const int FID_SW_SECTOR_NAME = FID_BOND_PUBLIC_BASE + 253;   //行业名字
const int FID_SW_SECTOR_CODE = FID_BOND_PUBLIC_BASE + 254;   //行业代码
const int FID_SW_SECTOR_UPPER_CODE = FID_BOND_PUBLIC_BASE + 255;   //二级行业所属的上一级行业代码
const int FID_ISSUE_PROVINCE = FID_BOND_PUBLIC_BASE + 256;   //bond发行省份
const int FID_ISSUE_PROVINCE_CODE = FID_BOND_PUBLIC_BASE + 257;   //bond发行省份代码
const int FID_ISSUING_INSTITUTION = FID_BOND_PUBLIC_BASE + 258;   //bond发行人
const int FID_SHARED_GROUP_QUOTE_WARNING = FID_BOND_PUBLIC_BASE + 259;   //共享群报价机构交易员没有绑定提示
const int FID_CITY_NAME = FID_BOND_PUBLIC_BASE + 260;  // 城市
const int FID_FINANCING_PLATFORM = FID_BOND_PUBLIC_BASE + 261;  // 平台债
const int FID_COMPANY_BOND = FID_BOND_PUBLIC_BASE + 262;  // 公司债
const int FID_DURATION_FILTER = FID_BOND_PUBLIC_BASE + 263;  // 久期过滤
const int FID_VAL_CONVEXITY = FID_BOND_PUBLIC_BASE + 264;  // 凸性
const int FID_VAL_BASIC_POINT_VALUE = FID_BOND_PUBLIC_BASE + 265;  // PVBP
const int FID_ISSUE_INSTITUTION_RATING = FID_BOND_PUBLIC_BASE + 266;  // 发行人级别
const int FID_OUTLOOK = FID_BOND_PUBLIC_BASE + 267;  // 展望 Add by Young 20-Apr-2016
const int FID_LISTED_TYPE = FID_BOND_PUBLIC_BASE + 268;
const int FID_LISTED_ON_MARKET = FID_BOND_PUBLIC_BASE + 269;
const int FID_CUSTOM_BOND_TYPE = FID_BOND_PUBLIC_BASE + 270;
const int FID_OPTION_CLIENT = FID_BOND_PUBLIC_BASE + 271;
const int FID_BEST_QUOTE_COUNT = FID_BOND_PUBLIC_BASE + 272; // 2272 单边:1 , 双边:2
const int FID_ISSUE_RATE = FID_BOND_PUBLIC_BASE + 273;
const int FID_ASSET_STATUS = FID_BOND_PUBLIC_BASE + 274;
const int FID_ROW_COUNT2 = FID_BOND_PUBLIC_BASE + 275;
const int FID_CLEAR_SPEED = FID_BOND_PUBLIC_BASE + 276;
const int FID_CREATE_TIME_MILLISECOND = FID_BOND_PUBLIC_BASE + 277;
const int FID_MODIFY_TIME_MILLISECOND = FID_BOND_PUBLIC_BASE + 278;
const int FID_DEAL_TIME_MILLISECOND = FID_BOND_PUBLIC_BASE + 279;
const int FID_SOURCE = FID_BOND_PUBLIC_BASE + 280;  // 1 - idb, 2 - idc
const int FID_CHINA_BOND_INST_RATING = FID_BOND_PUBLIC_BASE + 281; // 中债资信评级
const int FID_CHINA_BOND_INST_RATING_ORDER = FID_BOND_PUBLIC_BASE + 282; // 中债资信评级排序
const int FID_CHINA_BOND_INST_RATING_FILTER = FID_BOND_PUBLIC_BASE + 283; // 中债资信评级筛选

const int FID_FROM_LEVEL1_TO_LEVEL2_MARKET = FID_BOND_PUBLIC_BASE + 300;

//----------------------  Bond Quote -----------------------------
const int FID_BOND_QUOTE_BASE = 3000;
const int FID_QUOTE_SIDE = FID_BOND_QUOTE_BASE + 1;  // "QuoteSide":报价方向, -1-ofr，1-bid
const int FID_QUOTE_STATUS = FID_BOND_QUOTE_BASE + 2;  // "QuoteStatus":报价状态. 0 - 正常报价， 2 - referred的报价
const int FID_OCO_FLAG = FID_BOND_QUOTE_BASE + 3;  // "OCOFlag":OCO标识, 0-代表不显示，1-显示oco  2-oco打包
const int FID_VIP_FLAG = FID_BOND_QUOTE_BASE + 4;  // "VIPFlag":VIP标识, 是否VIP，0-否 1-是
const int FID_BAD_FLAG = FID_BOND_QUOTE_BASE + 5;  // "BadFlag":好/坏 地雷 
const int FID_CALCULATOR_ON = FID_BOND_QUOTE_BASE + 6;  // "CalculatorOn":是否开启计算器, false-不开启，true-开启计算器
const int FID_COMPARISON_SYMBOL = FID_BOND_QUOTE_BASE + 7;  // "ComparisonSymbol":与中债估值的比较符号，比如>=,  <=
const int FID_HAVING_QUOTE = FID_BOND_QUOTE_BASE + 8;  // "HavingQuote":与是否有报价
const int FID_TWO_SIDED = FID_BOND_QUOTE_BASE + 9;  // "TwoSided":是否双边
const int FID_AVG_PRICE = FID_BOND_QUOTE_BASE + 10; // "AvgPrice":交易均价
const int FID_CHECK_VALUATION = FID_BOND_QUOTE_BASE + 11; // "CheckValuation":和中债估值比较
const int FID_DIFF_WITH_VALUATION = FID_BOND_QUOTE_BASE + 12; // "DiffWithValuation":和中债估值的偏差
const int FID_QUOTE_INTERNAL_FLAG = FID_BOND_QUOTE_BASE + 13; // "QuoteInternalFlag":1-正常报价，2-内部报价
const int FID_QUOTE_ID = FID_BOND_QUOTE_BASE + 14; // "QuoteID": 报价ID
const int FID_BARGAIN_FLAG = FID_BOND_QUOTE_BASE + 15; // "BargainFlag":议价标识
const int FID_REFER_TIME = FID_BOND_QUOTE_BASE + 16; // "ReferTime":Refer Time
const int FID_PRICE_DESCRIPTION = FID_BOND_QUOTE_BASE + 17; // "PriceDescription":价格备注
const int FID_QUOTE_TO_DCS = FID_BOND_QUOTE_BASE + 18;	// "quoteToDcs" : 报价gvn/tkn时是否同时增加一笔成交到DCS
const int FID_QUOTE_PRICE = FID_BOND_QUOTE_BASE + 19;	// quotePrice: 目前仅IDC.GvnTknQuote时传入该字段，用于校验数据是否一致
const int FID_DIFF_WITH_NETPRICE_VALUATION = FID_BOND_QUOTE_BASE + 20; // 净价偏差

//----------------------  Oper log -----------------------------
const int FID_OPER_LOG_BASE = 3200;
const int FID_OPER_TIME = FID_OPER_LOG_BASE + 1;						  // "OperTime":操作时间
const int FID_OPER_USERNAME = FID_OPER_LOG_BASE + 2;					  // "OperUsername":操作人
const int FID_OPER_TYPE = FID_OPER_LOG_BASE + 3;						  // "OperType":操作类型 1-线下；2-线上
const int FID_OPER_SOURCE = FID_OPER_LOG_BASE + 4;	  // 1 - idb, 2 - idc
const int FID_OPER_SOURCE_TYPE = FID_OPER_LOG_BASE + 5;
const int FID_OPER_ID = FID_OPER_LOG_BASE + 6;

//----------------------  Bond Deal -----------------------------
const int FID_BOND_DEAL_BASE = 4000;
const int FID_DEAL_ID = FID_BOND_DEAL_BASE + 0;   // "DealID":成交ID
const int FID_DEAL_NO = FID_BOND_DEAL_BASE + 1;   // "DealNo":成交单号
const int FID_DIRECTION = FID_BOND_DEAL_BASE + 2;   // "Direction":买卖方向, 买卖方向, GVN/TKN／TRD
const int FID_DEAL_INTERNAL_FLAG = FID_BOND_DEAL_BASE + 3;   // "DealInternalFlag":1-非内部成交，2-内部成交
const int FID_DEAL_STATUS = FID_BOND_DEAL_BASE + 4;   // "DealStatus":成交单状态, 0-确认，1-待提交，2-已提交，3-已通过，5-未通过，7-已毁单，13-送审中，-1-已删除

const int FID_BID_INSTITUTION_ID = FID_BOND_DEAL_BASE + 5;   // "BidInstitutionId":Bid方机构ID
const int FID_BID_INSTITUTION_NAME = FID_BOND_DEAL_BASE + 6;   // "BidInstitutionName":Bid方机构名称
const int FID_BID_INSTITUTION_CITY = FID_BOND_DEAL_BASE + 7;   // "BidInstitutionCity":Bid方机构所在城市
const int FID_BID_TRADER_ID = FID_BOND_DEAL_BASE + 8;   // "BidTraderID":Bid方交易员ID
const int FID_BID_TRADER_NAME = FID_BOND_DEAL_BASE + 9;   // "BidTraderName":Bid方交易员姓名
const int FID_BID_BROKER_ID = FID_BOND_DEAL_BASE + 10;  // "BidBrokerId":Bid方经纪人ID
const int FID_BID_BROKER_NAME = FID_BOND_DEAL_BASE + 11;  // "BidBrokerName":Bid方经纪人名字
const int FID_BID_BROKERAGE = FID_BOND_DEAL_BASE + 12;  // "BidBrokerage":Bid方佣金
const int FID_BID_IS_BRIDGE = FID_BOND_DEAL_BASE + 13;  // "BidIsBridge":Bid方是否为过桥机构
const int FID_BID_IS_CHARGED = FID_BOND_DEAL_BASE + 14;  // "BidIsCharged":Bid方是否付佣金
const int FID_BID_BROKER_CHECKED = FID_BOND_DEAL_BASE + 15;  // "BidBrokerChecked":Bid方经纪人是否已经确认
const int FID_BID_BROKER_CHECK_DATE = FID_BOND_DEAL_BASE + 16;  // "BidBrokerCheckDate":Bid方经纪人确认时间
const int FID_BID_TRADER_CHECKED = FID_BOND_DEAL_BASE + 17;  // "BidTraderChecked":Bid方交易员是否已经确认
const int FID_BID_TRADER_CHECKDATE = FID_BOND_DEAL_BASE + 18;  // "BidTraderCheckDate":Bid方交易员确认时间

const int FID_BRIDGE_INSTITUTION_ID = FID_BOND_DEAL_BASE + 19;  // "BridgeInstitutionId":过桥机构ID
const int FID_BRIDGE_INSTITUTION_NAME = FID_BOND_DEAL_BASE + 20;  // "BridgeInstitutionName":过桥机构代码
const int FID_BRIDGE_TRADER_ID = FID_BOND_DEAL_BASE + 21;  // "BridgeTraderId":过桥机构交易员ID
const int FID_BRIDGE_TRADER_NAME = FID_BOND_DEAL_BASE + 22;  // "BridgeTraderName":过桥机构交易员代码
const int FID_BRIDGE_IS_CHARGED = FID_BOND_DEAL_BASE + 23;  // "BridgeIsCharged":过桥机构是否付佣金

const int FID_OFR_INSTITUTION_ID = FID_BOND_DEAL_BASE + 24;  // "OfrInstitutionId":Ofr方机构ID
const int FID_OFR_INSTITUTION_NAME = FID_BOND_DEAL_BASE + 25;  // "OfrInstitutionName":Ofr方机构名称
const int FID_OFR_INSTITUTION_CITY = FID_BOND_DEAL_BASE + 26;  // "OfrInstitutionCity":Ofr方机构所在城市
const int FID_OFR_TRADER_ID = FID_BOND_DEAL_BASE + 27;  // "OfrTraderID":Ofr方交易员ID
const int FID_OFR_TRADER_NAME = FID_BOND_DEAL_BASE + 28;  // "OfrTraderName":OFR方交易员姓名
const int FID_OFR_BROKER_ID = FID_BOND_DEAL_BASE + 29;  // "OfrBrokerId":Ofr方经纪人ID
const int FID_OFR_BROKER_NAME = FID_BOND_DEAL_BASE + 30;  // "OfrBrokerName":Ofr方经纪人名字
const int FID_OFR_BROKERAGE = FID_BOND_DEAL_BASE + 31;  // "OfrBrokerage":Ofr方佣金
const int FID_OFR_IS_BRIDGE = FID_BOND_DEAL_BASE + 32;  // "OfrIsBridge":Ofr方是否为过桥机构
const int FID_OFR_IS_CHARGED = FID_BOND_DEAL_BASE + 33;  // "OfrIsCharged":Ofr方是否付佣金
const int FID_OFR_BROKER_CHECKED = FID_BOND_DEAL_BASE + 34;  // "OfrBrokerChecked":Ofr方经纪人是否已经确认
const int FID_OFR_BROKER_CHECK_DATE = FID_BOND_DEAL_BASE + 35;  // "OfrBrokerCheckDate":Ofr方经纪人确认时间
const int FID_OFR_TRADER_CHECKED = FID_BOND_DEAL_BASE + 36;  // "OfrTraderChecked":Ofr方交易员是否已经确认
const int FID_OFR_TRADER_CHECK_DATE = FID_BOND_DEAL_BASE + 37;  // "OfrTraderCheckDate":Ofr方交易员确认时间

const int FID_CURRENCY = FID_BOND_DEAL_BASE + 38;  // "Currency":货币, 货币, USD/CNY…
const int FID_SETTLEMENT_DATE = FID_BOND_DEAL_BASE + 39;  // "SettlementDate":结算日
const int FID_SETTLEMENT_MODE = FID_BOND_DEAL_BASE + 40;  // "SettlementMode":结算模式, 如DVP
const int FID_SETTLEMENT_AMOUNT = FID_BOND_DEAL_BASE + 41;  // "SettlementAmount":结算金额

const int FID_DEAL_TIME = FID_BOND_DEAL_BASE + 42;  // "DealTime":成交时间
const int FID_CONFIRM_DATE = FID_BOND_DEAL_BASE + 43;  // "ConfirmDate"：最终（经理）确认日期
const int FID_CONFIRM_ACCOUNT = FID_BOND_DEAL_BASE + 44;  // "ConfirmAccount":最终（经理）确认日期
const int FID_BID_INSTITUTION_TRADER_NAMES = FID_BOND_DEAL_BASE + 45;  // "BidInstitutionTraderNames":买方机构(交易员)，比如：农行北京（徐明）
const int FID_IS_PRINT = FID_BOND_DEAL_BASE + 46;  // "IsPrint":是否已经打印, 1-已经打印
const int FID_GROUP_FLAG = FID_BOND_DEAL_BASE + 47;  // "GroupFlag":过桥的两个成交单的标识, 如果没有过桥，则为NULL
const int FID_TRADED_DATE = FID_BOND_DEAL_BASE + 48;  // "TradedDate":交易日期
const int FID_REMARKS = FID_BOND_DEAL_BASE + 49;  // "Remarks":评论
const int FID_SEND_STATUS = FID_BOND_DEAL_BASE + 50;  // "SendStatus":消息推送的状态, 2：停止；3：重新计时；1：立即发送
const int FID_OFR_INSTITUTION_TRADER_NAMES = FID_BOND_DEAL_BASE + 51;  // "OfrInstitutionTraderNames":卖方机构(交易员)，比如：农行北京（徐明）
const int FID_BID_INSTITUTION_CODE = FID_BOND_DEAL_BASE + 52;  // "BidInstitutionCode":Bid方机构代码
const int FID_OFR_INSTITUTION_CODE = FID_BOND_DEAL_BASE + 53;  // "OfrInstitutionCode":Ofr方机构代码
const int FID_BID_CHECK = FID_BOND_DEAL_BASE + 54;  // "BidCheck":Bid方确认标识
const int FID_OFR_CHECK = FID_BOND_DEAL_BASE + 55;  // "OfrCheck":Ofr方确认标识
const int FID_BID_CHECK_DATE = FID_BOND_DEAL_BASE + 56;  // "BidCheckDate":Bid方确认时间
const int FID_OFR_CHECK_DATE = FID_BOND_DEAL_BASE + 57;  // "OfrCheckDate":Ofr方确认时间
const int FID_DEAL_STATUS_STRING = FID_BOND_DEAL_BASE + 58;  // "DealStatusString":成交单确认状态
const int FID_SERIAL_NUMBER = FID_BOND_DEAL_BASE + 59;  // "SerialNumber":成交单序列号
const int FID_BID_REMARKS = FID_BOND_DEAL_BASE + 60;  // "BidRemarks": Bid备注
const int FID_OFR_REMARKS = FID_BOND_DEAL_BASE + 61;  // "OfrRemarks": Ofr备注
const int FID_BID_BROKERAGE_RATE = FID_BOND_DEAL_BASE + 62;  // "BidBrokerageRate": Bid佣金比例
const int FID_OFR_BROKERAGE_RATE = FID_BOND_DEAL_BASE + 63;  // "OfrBrokerageRate": Ofr佣金比例
const int FID_BID_DISCOUNT = FID_BOND_DEAL_BASE + 64;  // "BidDiscount": Bid佣金折扣
const int FID_OFR_DISCOUNT = FID_BOND_DEAL_BASE + 65;  // "OfrDiscount": Ofr佣金折扣

const int FID_BID_BROKER_CODE = FID_BOND_DEAL_BASE + 66;  // bid broker code
const int FID_OFR_BROKER_CODE = FID_BOND_DEAL_BASE + 67;  // bid broker code

const int FID_BID_BROKER_ID_A = FID_BOND_DEAL_BASE + 68;  // Bid方经纪人ID
const int FID_BID_BROKER_ID_B = FID_BOND_DEAL_BASE + 69;
const int FID_BID_BROKER_ID_C = FID_BOND_DEAL_BASE + 70;
const int FID_BID_BROKER_ID_D = FID_BOND_DEAL_BASE + 71;
const int FID_BID_BROKER_NAME_A = FID_BOND_DEAL_BASE + 72;  // Bid方经纪人名字
const int FID_BID_BROKER_NAME_B = FID_BOND_DEAL_BASE + 73;
const int FID_BID_BROKER_NAME_C = FID_BOND_DEAL_BASE + 74;
const int FID_BID_BROKER_NAME_D = FID_BOND_DEAL_BASE + 75;
const int FID_BID_PERCENT_A = FID_BOND_DEAL_BASE + 76;  // Bid方佣金比例
const int FID_BID_PERCENT_B = FID_BOND_DEAL_BASE + 77;
const int FID_BID_PERCENT_C = FID_BOND_DEAL_BASE + 78;
const int FID_BID_PERCENT_D = FID_BOND_DEAL_BASE + 79;
const int FID_OFR_BROKER_ID_A = FID_BOND_DEAL_BASE + 80;  // ofr方经纪人ID
const int FID_OFR_BROKER_ID_B = FID_BOND_DEAL_BASE + 81;
const int FID_OFR_BROKER_ID_C = FID_BOND_DEAL_BASE + 82;
const int FID_OFR_BROKER_ID_D = FID_BOND_DEAL_BASE + 83;
const int FID_OFR_BROKER_NAME_A = FID_BOND_DEAL_BASE + 84;  // ofr方经纪人名字
const int FID_OFR_BROKER_NAME_B = FID_BOND_DEAL_BASE + 85;
const int FID_OFR_BROKER_NAME_C = FID_BOND_DEAL_BASE + 86;
const int FID_OFR_BROKER_NAME_D = FID_BOND_DEAL_BASE + 87;
const int FID_OFR_PERCENT_A = FID_BOND_DEAL_BASE + 88;  // ofr方佣金比例
const int FID_OFR_PERCENT_B = FID_BOND_DEAL_BASE + 89;
const int FID_OFR_PERCENT_C = FID_BOND_DEAL_BASE + 90;
const int FID_OFR_PERCENT_D = FID_BOND_DEAL_BASE + 91;
const int FID_PRIMARYMARKETFLAG = FID_BOND_DEAL_BASE + 92; // 客户录入，区分成交数据是一级还是二级
const int FID_HAVING_SERIAL_NUMBER = FID_BOND_DEAL_BASE + 93;	// 序列号是否有值
const int FID_BID_TRADER_CODE = FID_BOND_DEAL_BASE + 94;
const int FID_OFR_TRADER_CODE = FID_BOND_DEAL_BASE + 95;

const int FID_HAS_DEAL = FID_BOND_DEAL_BASE + 100;      // 客户查询某只券是否有成交

//----------------------  DCS BOND DEAL 相关字段 -----------------------------
const int FID_DCS_RESULT = FID_BOND_DEAL_BASE + 200;	// dcs 操作结果
const int FID_DCS_IN_HAND = FID_BOND_DEAL_BASE + 201;	// true:正在处理中 false:正常状态
const int FID_BID_TRADE_MODE = FID_BOND_DEAL_BASE + 202;	// dcs bid方交易方式
const int FID_BID_IS_NO_BROKERAGE = FID_BOND_DEAL_BASE + 203;	// Bid方是否免佣
const int FID_BID_NO_CONFIRM = FID_BOND_DEAL_BASE + 204;	// Bid方NC
const int FID_BID_NO_COMMENTS = FID_BOND_DEAL_BASE + 205;	// Bid方NC备注
const int FID_OFR_TRADE_MODE = FID_BOND_DEAL_BASE + 206;	// dcs ofr方交易方式
const int FID_OFR_IS_NO_BROKERAGE = FID_BOND_DEAL_BASE + 207;	// Ofr方是否免佣
const int FID_OFR_NO_CONFIRM = FID_BOND_DEAL_BASE + 208;	// Ofr方NC
const int FID_OFR_NO_COMMENTS = FID_BOND_DEAL_BASE + 209;	// Ofr方NC备注
const int FID_BID_SPECIAL_PARTY = FID_BOND_DEAL_BASE + 210;	// bid方特别细节
const int FID_OFR_SPECIAL_PARTY = FID_BOND_DEAL_BASE + 211;	// bid方特别细节
const int FID_DCS_SPECIAL_TERMS = FID_BOND_DEAL_BASE + 212;	// 其他细节
const int FID_FEEDBACK = FID_BOND_DEAL_BASE + 213;	// dcs 后台反馈
const int FID_SWITCH_STATUS = FID_BOND_DEAL_BASE + 214;	// dcs switch status

// dcs 提交失败或毁单失败消息推送
const int FID_DCS_FAIL_DCS_ID = FID_BOND_DEAL_BASE + 215;	// 成交单ID
const int FID_DCS_FAIL_MSG = FID_BOND_DEAL_BASE + 216;	// 返回消息
const int FID_DCS_FAIL_OPERATE_TYPE = FID_BOND_DEAL_BASE + 217;	// 操作类型 submit/destroy
const int FID_DCS_FAIL_OPERATE_NAME = FID_BOND_DEAL_BASE + 218;	// 操作人

//dcs成交单历史操作详情
const int FID_DCS_HISTORY_OPERATE = FID_BOND_DEAL_BASE + 219;	// 操作
const int FID_DCS_HISTORY_OPERATOR = FID_BOND_DEAL_BASE + 220;	// 操作人
const int FID_DCS_HISTORY_UPDATE_TIME = FID_BOND_DEAL_BASE + 221;	// 操作时间
const int FID_DCS_HISTORY_COMMENT = FID_BOND_DEAL_BASE + 222;	// 备注
const int FID_DCS_HISTORY_UPDATE_CONTENT = FID_BOND_DEAL_BASE + 223;	// 更改内容

const int FID_RELATE_SUBMIT_COMMENTS = FID_BOND_DEAL_BASE + 224;	// 关联提交备注
const int FID_URGENT_STATUS = FID_BOND_DEAL_BASE + 225;	// 加急标识
const int FID_OLD_CONTRACT_ID = FID_BOND_DEAL_BASE + 226;	// 编辑过桥拆单时，老成交单的ID
const int FID_RELATED_MSG = FID_BOND_DEAL_BASE + 227;	// 过桥关联字段
const int FID_BID_TRADER_ACCOUNT = FID_BOND_DEAL_BASE + 228;	// Bid方交易员账户名
const int FID_OFR_TRADER_ACCOUNT = FID_BOND_DEAL_BASE + 229;	// Ofr方交易员账户名
const int FID_DCS_QUESTION_MARK = FID_BOND_DEAL_BASE + 230;	// 问号标记

const int FID_DCS_DESTROY_REASON = FID_BOND_DEAL_BASE + 231;	// 毁单原因

const int FID_DCS_QB_STATUS = FID_BOND_DEAL_BASE + 232;	// dcs成交单推送qb状态
const int FID_DCS_BID_BANK_QB_ID = FID_BOND_DEAL_BASE + 233;	// dcs deal bid_bank qb_id
const int FID_DCS_BID_AGENT_QB_ID = FID_BOND_DEAL_BASE + 234;	// dcs deal bid_trader qb_id
const int FID_DCS_OFR_BANK_QB_ID = FID_BOND_DEAL_BASE + 235;	// dcs deal ofr_bank qb_id
const int FID_DCS_OFR_AGENT_QB_ID = FID_BOND_DEAL_BASE + 236;	// dcs deal ofr_trader qb_id

const int FID_BID_PAY_FOR_ID = FID_BOND_DEAL_BASE + 237;           // Bid方被代付机构
const int FID_OFR_PAY_FOR_ID = FID_BOND_DEAL_BASE + 238;           // Ofr方被代付机构
const int FID_BID_PAY_FOR_NAME = FID_BOND_DEAL_BASE + 239;      // Bid方被代付机构名字
const int FID_OFR_PAY_FOR_NAME = FID_BOND_DEAL_BASE + 240;      // Ofr方被代付机构名字
const int FID_PAYFOR = FID_BOND_DEAL_BASE + 241;      // 代付信息
const int FID_BID_PAYFOR = FID_BOND_DEAL_BASE + 242;      // Bid方代付信息
const int FID_OFR_PAYFOR = FID_BOND_DEAL_BASE + 243;      // Ofr方代付信息
const int FID_HAS_RELATED_MSG = FID_BOND_DEAL_BASE + 244;	// 是否有过桥码（过桥码字段非空）
const int FID_IS_CHANGE_STATUS = FID_BOND_DEAL_BASE + 245; // 更新dcs成交单时是否改变状态，用以区分调用dcs的哪个更新接口
const int FID_DCS_SEQ = FID_BOND_DEAL_BASE + 246;	// 内码，IDC新增的dcs成交单会设置该字段，bond中不会设置及修改该字段
const int FID_HAVING_DCS_SEQ = FID_BOND_DEAL_BASE + 247;

//----------------------  Product Favor -----------------------------
const int FID_BOND_GROUP_BASE = 5000;
const int FID_GROUP_NAME = FID_BOND_GROUP_BASE + 0;  // "GroupName":分组名

const int FID_GROUP_ID = FID_BOND_GROUP_BASE + 3;  // "GroupId":分组ID
const int FID_USER_ACCOUNT_ID = FID_BOND_GROUP_BASE + 4;  // "UserAccountId":客户ID
const int FID_GROUP_FILTER = FID_BOND_GROUP_BASE + 5;  // "GroupFilter": 分组过滤条件
const int FID_GROUP_KEY = FID_BOND_GROUP_BASE + 6;

//----------------------  Calculator  -----------------------------
const int FID_CALCULATOR_BASE = 6000;
const int FID_INTEREST_START_DATE = FID_CALCULATOR_BASE + 0;   // "InterestStartDate":起息日
const int FID_FIRST_COUPON_DATE = FID_CALCULATOR_BASE + 1;   // "FirstCouponDate":第一次付息日
const int FID_COUPON_DAY_ADJUST = FID_CALCULATOR_BASE + 2;   // "CouponDayAdjust":付息日是否调整
const int FID_COUPON_DAY_DMC = FID_CALCULATOR_BASE + 3;   // "CouponDayDMC":付息日调整规则
const int FID_COUPON_CALENDAR_KEY = FID_CALCULATOR_BASE + 4;   // "CouponCalendarKey":付息日调整日历
const int FID_MATURITY_ADJUST = FID_CALCULATOR_BASE + 5;   // "MaturityAdjust:"到期日是否调整
const int FID_MATURITY_DMC = FID_CALCULATOR_BASE + 6;   // "MaturityDMC":到期日调整规则
const int FID_MATURITY_CALENDAR_KEY = FID_CALCULATOR_BASE + 7;   // "MaturityCalendarKey":到期日调整日历
const int FID_COUPON_TYPE = FID_CALCULATOR_BASE + 8;   // "CouponType":债券付息类型
const int FID_COUPON_FREQUENCY = FID_CALCULATOR_BASE + 9;   // "CouponFrequency":付息频率
const int FID_COUPON_RATE_SPREAD = FID_CALCULATOR_BASE + 10;  // "CouponRateSpread":票面利率/利差
const int FID_ISSUE_PRICE = FID_CALCULATOR_BASE + 11;  // "IssuePrice":发行价
const int FID_INTEREST_BASIS = FID_CALCULATOR_BASE + 12;  // "InterestBasis":利息规则
const int FID_SIMPLE_COMPOUND = FID_CALCULATOR_BASE + 13;  // "SimpleCompound":单利/复利
const int FID_COMPOUND_FREQUENCY = FID_CALCULATOR_BASE + 14;  // "CompoundFrequency":复利频率
const int FID_FRN_MULTIPLIER = FID_CALCULATOR_BASE + 15;  // "FRNMultiplier":基准利率倍率
const int FID_FRN_INDEX_ID = FID_CALCULATOR_BASE + 16;  // "FRNIndexID":基准利率ID
const int FID_FIRST_INDEX_RATE = FID_CALCULATOR_BASE + 17;  // "FirstIndexRate":首次付息利率
const int FID_FIXING_FREQUENCY = FID_CALCULATOR_BASE + 18;  // "FixingFrequency":基准利率固定频率
const int FID_FIXING_MA_DAYS = FID_CALCULATOR_BASE + 19;  // "FixingMADays":基准利率移动平均天数
const int FID_FIXING_PRECEDS = FID_CALCULATOR_BASE + 20;  // "FixingPreceds":基准利率固定提前天数
const int FID_FIXING_CALENDAR_KEY = FID_CALCULATOR_BASE + 21;  // "FixingCalendarKey":基准利率固定日历
const int FID_FIXING_DIGIT = FID_CALCULATOR_BASE + 22;  // "FixingDigit":基准利率保留小数位数
const int FID_RESET_EFFECTIVE = FID_CALCULATOR_BASE + 23;  // "ResetEffective":重置生效
const int FID_REDEMPTION_NO = FID_CALCULATOR_BASE + 24;  // "RedemptionNo":提前还本期数
const int FID_REDEMPTION_STR = FID_CALCULATOR_BASE + 25;  // "RedemptionStr":提前还本日期及金额
const int FID_COMPENSATE_RATE = FID_CALCULATOR_BASE + 26;  // "CompensateRate":补偿利率范围
const int FID_COMPENSATE_FROM = FID_CALCULATOR_BASE + 27;  // "CompensateFrom":补偿利率起始期数
const int FID_OPTION_STYLE = FID_CALCULATOR_BASE + 28;  // "OptionStyle":欧式/美式
const int FID_CALL_NO = FID_CALCULATOR_BASE + 29;  // "CallNo":赎回次数
const int FID_CALL_STR = FID_CALCULATOR_BASE + 30;  // "CallStr":赎回日期及赎回价格
const int FID_PUT_NO = FID_CALCULATOR_BASE + 31;  // "PutNo":回售次数
const int FID_PUT_STR = FID_CALCULATOR_BASE + 32;  // "PutStr":回售日期及回售价格
const int FID_ASSIGN_TRANSSTR = FID_CALCULATOR_BASE + 33;  // "AssignTransStr":定向转让日期及价格
const int FID_CAP = FID_CALCULATOR_BASE + 34;  // "Cap":浮动利率上限
const int FID_FLOOR = FID_CALCULATOR_BASE + 35;  // "Floor":浮动利率下限

const int FID_IS_FRN_BOND = FID_CALCULATOR_BASE + 36;  // "IsFRNBond": 是否为浮动债券
const int FID_IS_YTC_AVAILABLE = FID_CALCULATOR_BASE + 37;  // "IsYTCAvailable": 是否可赎回
const int FID_IS_YTP_AVAILABLE = FID_CALCULATOR_BASE + 38;  // "IsYTPAvailable": 是否可回售
const int FID_INDEX_RATE = FID_CALCULATOR_BASE + 39;  // "IndexRate":基准利率值
const int FID_INDEX_DATE = FID_CALCULATOR_BASE + 40;  // "IndexDate":基准利率取值日期
const int FID_YIELD_TO_MATURITY = FID_CALCULATOR_BASE + 41;  // "YieldToMaturity":到期收益率
const int FID_YIELD_TO_CALL = FID_CALCULATOR_BASE + 42;  // "YieldToCall":行权收益率（赎回）
const int FID_YIELD_TO_PUT = FID_CALCULATOR_BASE + 43;  // "YieldToPut":行权收益率（回售）
const int FID_ACCRUED_DAYS = FID_CALCULATOR_BASE + 44;  // "AccruedDays":计息天数
const int FID_ACCRUED_AMOUNT = FID_CALCULATOR_BASE + 45;  // "AccruedAmount":应计利息
const int FID_EXECUTION_DATE = FID_CALCULATOR_BASE + 46;  // "ExecutionDate":行权日期
const int FID_MACAULAY_DURATION = FID_CALCULATOR_BASE + 47;  // "MacaulayDuration":Macauly久期
const int FID_MODIFIED_DURATION = FID_CALCULATOR_BASE + 48;  // "ModifiedDuration":修正久期
const int FID_PVBP = FID_CALCULATOR_BASE + 49;  // "PVBP":PVBP
const int FID_CONVEXITY = FID_CALCULATOR_BASE + 50;  // "Convexity":凸性
const int FID_CALCULATOR_ERROR_MESSAGE = FID_CALCULATOR_BASE + 51;  // "CalculatorErrorMessage":错误信息
const int FID_CALCULATE_ACTION_TYPE = FID_CALCULATOR_BASE + 52;  // "CalculateActionType":计算动作类型
const int FID_CALENDAR_MARKET_KEY = FID_CALCULATOR_BASE + 53;  // "CalendarMarketKey":日历市场
const int FID_DATE = FID_CALCULATOR_BASE + 54;  // "Date" 日期
const int FID_PERIOD_NUM = FID_CALCULATOR_BASE + 55;  // "PeriodNum" 日期调整数字
const int FID_PERIOD_UNIT = FID_CALCULATOR_BASE + 56;  // "PeriodNum" 日期调整单位
const int FID_BUSINESS_DAY_CONVENTION = FID_CALCULATOR_BASE + 57;  // "BusinessDayConvention" 日期调整规则
const int FID_YIELD_TO_EXECUTION = FID_CALCULATOR_BASE + 58;  // "YieldToExecution":行权收益率
const int FID_TENOR = FID_CALCULATOR_BASE + 59;  // "Tenor":"1D"
const int FID_CALCULATOR_ERROR_CODE = FID_CALCULATOR_BASE + 60;  // "CalculatorErrorMessage":错误信息

//----------------------  Best Quote -----------------------------
const int FID_BOND_BEST_QUOTE_BASE = 7000;
const int FID_BEST_BID_VOLUME = FID_BOND_BEST_QUOTE_BASE + 0;  // "BestBidVolume":Bid数量，单位（元）
const int FID_BEST_OFR_VOLUME = FID_BOND_BEST_QUOTE_BASE + 1;  // "BestOfrVolume":Ofr数量，单位（元）
const int FID_BEST_BID_PRICE = FID_BOND_BEST_QUOTE_BASE + 2;  // "BestBidPrice":最优Bid报价
const int FID_BEST_OFR_PRICE = FID_BOND_BEST_QUOTE_BASE + 3;  // "BestOfrPrice":最优Ofr报价
const int FID_BEST_BID_QUOTE_COUNT = FID_BOND_BEST_QUOTE_BASE + 4;  // "BestBidQuoteCount":Bid最优报价个数
const int FID_BEST_OFR_QUOTE_COUNT = FID_BOND_BEST_QUOTE_BASE + 5;  // "BestOfrQuoteCount":Ofr最优报价个数
const int FID_BEST_BID_DIRTY_PRICE = FID_BOND_BEST_QUOTE_BASE + 6;  // "BestBidDirtyPrice":Bid全价
const int FID_BEST_OFR_DIRTY_PRICE = FID_BOND_BEST_QUOTE_BASE + 7;  // "BestOfrDirtyPrice":Ofr全价
const int FID_BEST_BID_CLEAN_PRICE = FID_BOND_BEST_QUOTE_BASE + 8;  // "BestBidCleanPrice":Bid净价
const int FID_BEST_OFR_CLEAN_PRICE = FID_BOND_BEST_QUOTE_BASE + 9;  // "BestOfrCleanPrice":Ofr净价
const int FID_BEST_BID_SPREAD = FID_BOND_BEST_QUOTE_BASE + 10; // "BestBidSpread":Bid利差
const int FID_BEST_OFR_SPREAD = FID_BOND_BEST_QUOTE_BASE + 11; // "BestOfrSpread":Ofr利差
const int FID_BID_QUOTE_COUNT = FID_BOND_BEST_QUOTE_BASE + 12; // "BidQuoteCount":Bid报价个数
const int FID_BEST_INTERNAL_BID_PRICE = FID_BOND_BEST_QUOTE_BASE + 13; // "InternalBidPrice": 最优报价中的内部Bid报价
const int FID_BEST_INTERNAL_OFR_PRICE = FID_BOND_BEST_QUOTE_BASE + 14; // "InternalOfrPrice": 最优报价中的内部Ofr报价
const int FID_BEST_INTERNAL_BID_FANDIAN = FID_BOND_BEST_QUOTE_BASE + 15; // "InternalBidFanDian": 最优报价中的内部Bid返点
const int FID_BEST_INTERNAL_OFR_FANDIAN = FID_BOND_BEST_QUOTE_BASE + 16; // "InternalOfrFanDian": 最优报价中的内部Ofr返点
const int FID_BEST_INTERNAL_BID_FANDIAN_FLAG = FID_BOND_BEST_QUOTE_BASE + 17; // "InternalBidFanDianFlag": 最优报价中的内部Bid是否返点
const int FID_BEST_INTERNAL_OFR_FANDIAN_FLAG = FID_BOND_BEST_QUOTE_BASE + 18; // "InternalOfrFanDianFlag": 最优报价中的内部Ofr是否返点
const int FID_COMPLETED_QUOTE_SORT = FID_BOND_BEST_QUOTE_BASE + 19; // "CompletedQuoteSort": 完整报价中排序字段
const int FID_COMPLETED_BOND_TYPE = FID_BOND_BEST_QUOTE_BASE + 20; // "CompletedBondType": 完整报价中债券类型
const int FID_OFR_QUOTE_COUNT = FID_BOND_BEST_QUOTE_BASE + 21; // "OfrQuoteCount": Ofr最优报价个数

const int FID_BID_QUOTE_TYPE = FID_BOND_BEST_QUOTE_BASE + 24; // "BidQuoteType":Bid报价类型
const int FID_OFR_QUOTE_TYPE = FID_BOND_BEST_QUOTE_BASE + 25; // "OfrQuoteType": Ofr报价类型
const int FID_BID_YIELD = FID_BOND_BEST_QUOTE_BASE + 26; // "BidYield": Bid收益率
const int FID_OFR_YIELD = FID_BOND_BEST_QUOTE_BASE + 27; // "OfrYield": Ofr收益率
const int FID_BID_SETTLEMENT_TYPE = FID_BOND_BEST_QUOTE_BASE + 28; // "BidSettlementType": Bid结算类型，T+0,T+1
const int FID_OFR_SETTLEMENT_TYPE = FID_BOND_BEST_QUOTE_BASE + 29; // "OfrSettlementType": OFR结算类型，T+0,T+1
const int FID_BID_DELIVERY_DATE = FID_BOND_BEST_QUOTE_BASE + 30; // "BidDeliveryDate": BID交割日
const int FID_OFR_DELIVERY_DATE = FID_BOND_BEST_QUOTE_BASE + 31; // "BidDeliveryDate": OFR交割日
const int FID_BID_YIELD_TYPE = FID_BOND_BEST_QUOTE_BASE + 32; // "BidYieldType": BID收益率类型0-行权收益率，1-到期收益率
const int FID_OFR_YIELD_TYPE = FID_BOND_BEST_QUOTE_BASE + 33; // "OfrYieldType": OFR收益率类型0-行权收益率，1-到期收益率
const int FID_INTERNAL_BID_QUOTE_ID = FID_BOND_BEST_QUOTE_BASE + 34; // "InternalBidQuoteID": 内部Bid报价ID
const int FID_INTERNAL_OFR_QUOTE_ID = FID_BOND_BEST_QUOTE_BASE + 35; // "InternalOfrQuoteID": 内部Ofr报价ID
const int FID_BID_BEST_QUOTE_ID = FID_BOND_BEST_QUOTE_BASE + 36; // "BidBestQuoteId": Bid最优报价ID
const int FID_OFR_BEST_QUOTE_ID = FID_BOND_BEST_QUOTE_BASE + 37; // "OfrBestQuoteId": Ofr最优报价ID
const int FID_IS_BEST_QUOTE = FID_BOND_BEST_QUOTE_BASE + 38; // "IsBestQuote": 最优报价 true 次优报价 false
const int FID_INTERNAL_BID_PRICE_STRING = FID_BOND_BEST_QUOTE_BASE + 39; // "InternalBidPriceString"
const int FID_INTERNAL_BID_FAN_DIAN_STRING = FID_BOND_BEST_QUOTE_BASE + 40; // "InternalBidFanDianString"
const int FID_INTERNAL_OFR_PRICE_STRING = FID_BOND_BEST_QUOTE_BASE + 41; // "InternalOfrPriceString"
const int FID_INTERNAL_OFR_FAN_DIAN_STRING = FID_BOND_BEST_QUOTE_BASE + 42; // "InternalOfrFanDianString"
const int FID_BEST_QUOTE_ID = FID_BOND_BEST_QUOTE_BASE + 43; // "BestQuoteId" 最优报价id
const int FID_BEST_BID_VOLUME_CLIENT = FID_BOND_BEST_QUOTE_BASE + 44;  // "BestBidVolumeClient": 所有最优报价数量累加值
const int FID_BEST_OFR_VOLUME_CLIENT = FID_BOND_BEST_QUOTE_BASE + 45;  // "BestOfrVolumeClient": 所有最优报价数量累加值

const int FID_HAVING_EXTERNAL_QUOTE = FID_BOND_BEST_QUOTE_BASE + 50;
const int FID_HAVING_INTERNAL_QUOTE = FID_BOND_BEST_QUOTE_BASE + 51;
const int FID_HAVING_BID_QUOTE = FID_BOND_BEST_QUOTE_BASE + 52;
const int FID_HAVING_OFR_QUOTE = FID_BOND_BEST_QUOTE_BASE + 53;
const int FID_BEST_BID_VOLUME_TOTAL = FID_BOND_BEST_QUOTE_BASE + 54;
const int FID_BEST_OFR_VOLUME_TOTAL = FID_BOND_BEST_QUOTE_BASE + 55;
const int FID_BEST_BID_DESCRIPTION = FID_BOND_BEST_QUOTE_BASE + 56;
const int FID_BEST_OFR_DESCRIPTION = FID_BOND_BEST_QUOTE_BASE + 57;

const int FID_BEST_BID_DEVIATE = FID_BOND_BEST_QUOTE_BASE + 58;
const int FID_BEST_OFR_DEVIATE = FID_BOND_BEST_QUOTE_BASE + 59;

const int FID_BEST_BID_LOCATION = FID_BOND_BEST_QUOTE_BASE + 60;
const int FID_BEST_OFR_LOCATION = FID_BOND_BEST_QUOTE_BASE + 61;
const int FID_BEST_INTERNAL_BID_LOCATION = FID_BOND_BEST_QUOTE_BASE + 62;
const int FID_BEST_INTERNAL_OFR_LOCATION = FID_BOND_BEST_QUOTE_BASE + 63;

const int FID_BEST_BID_IS_INTERNAL = FID_BOND_BEST_QUOTE_BASE + 64;
const int FID_BEST_OFR_IS_INTERNAL = FID_BOND_BEST_QUOTE_BASE + 65;
const int FID_BEST_UNION_BID_ID = FID_BOND_BEST_QUOTE_BASE + 66;
const int FID_BEST_UNION_BID_PRICE = FID_BOND_BEST_QUOTE_BASE + 67;
const int FID_BEST_UNION_BID_VOLUME = FID_BOND_BEST_QUOTE_BASE + 68;
const int FID_BEST_UNION_BID_FANDIAN = FID_BOND_BEST_QUOTE_BASE + 69;
const int FID_BEST_UNION_BID_OCO = FID_BOND_BEST_QUOTE_BASE + 70;
const int FID_BEST_UNION_BID_BARGIN = FID_BOND_BEST_QUOTE_BASE + 71;
const int FID_BEST_UNION_BID_DESC = FID_BOND_BEST_QUOTE_BASE + 72;
const int FID_BEST_UNION_BID_INST = FID_BOND_BEST_QUOTE_BASE + 73;
const int FID_BEST_UNION_BID_INST_ID = FID_BOND_BEST_QUOTE_BASE + 74;
const int FID_BEST_UNION_BID_TRADER = FID_BOND_BEST_QUOTE_BASE + 75;
const int FID_BEST_UNION_BID_TRADER_ID = FID_BOND_BEST_QUOTE_BASE + 76;
const int FID_BEST_UNION_BID_INST_TRADER = FID_BOND_BEST_QUOTE_BASE + 77;
const int FID_BEST_UNION_BID_BAD_FLAG = FID_BOND_BEST_QUOTE_BASE + 78;
const int FID_BEST_UNION_BID_TRADER_IS_VIP = FID_BOND_BEST_QUOTE_BASE + 79;
const int FID_BEST_UNION_BID_BROKER = FID_BOND_BEST_QUOTE_BASE + 80;
const int FID_BEST_UNION_BID_BROKER_ID = FID_BOND_BEST_QUOTE_BASE + 81;
const int FID_BEST_UNION_BID_FULL_PRICE = FID_BOND_BEST_QUOTE_BASE + 82;
const int FID_BEST_UNION_BID_NET_PRICE = FID_BOND_BEST_QUOTE_BASE + 83;
const int FID_BEST_UNION_BID_SPREAD = FID_BOND_BEST_QUOTE_BASE + 84;
const int FID_BEST_UNION_BID_QUOTE_TYPE = FID_BOND_BEST_QUOTE_BASE + 85;
const int FID_BEST_UNION_OFR_ID = FID_BOND_BEST_QUOTE_BASE + 86;
const int FID_BEST_UNION_OFR_PRICE = FID_BOND_BEST_QUOTE_BASE + 87;
const int FID_BEST_UNION_OFR_VOLUME = FID_BOND_BEST_QUOTE_BASE + 88;
const int FID_BEST_UNION_OFR_FANDIAN = FID_BOND_BEST_QUOTE_BASE + 89;
const int FID_BEST_UNION_OFR_OCO = FID_BOND_BEST_QUOTE_BASE + 90;
const int FID_BEST_UNION_OFR_BARGIN = FID_BOND_BEST_QUOTE_BASE + 91;
const int FID_BEST_UNION_OFR_DESC = FID_BOND_BEST_QUOTE_BASE + 92;
const int FID_BEST_UNION_OFR_INST = FID_BOND_BEST_QUOTE_BASE + 93;
const int FID_BEST_UNION_OFR_INST_ID = FID_BOND_BEST_QUOTE_BASE + 94;
const int FID_BEST_UNION_OFR_TRADER = FID_BOND_BEST_QUOTE_BASE + 95;
const int FID_BEST_UNION_OFR_TRADER_ID = FID_BOND_BEST_QUOTE_BASE + 96;
const int FID_BEST_UNION_OFR_INST_TRADER = FID_BOND_BEST_QUOTE_BASE + 97;
const int FID_BEST_UNION_OFR_BAD_FLAG = FID_BOND_BEST_QUOTE_BASE + 98;
const int FID_BEST_UNION_OFR_TRADER_IS_VIP = FID_BOND_BEST_QUOTE_BASE + 99;
const int FID_BEST_UNION_OFR_BROKER = FID_BOND_BEST_QUOTE_BASE + 100;
const int FID_BEST_UNION_OFR_BROKER_ID = FID_BOND_BEST_QUOTE_BASE + 101;
const int FID_BEST_UNION_OFR_FULL_PRICE = FID_BOND_BEST_QUOTE_BASE + 102;
const int FID_BEST_UNION_OFR_NET_PRICE = FID_BOND_BEST_QUOTE_BASE + 103;
const int FID_BEST_UNION_OFR_SPREAD = FID_BOND_BEST_QUOTE_BASE + 104;
const int FID_BEST_UNION_OFR_QUOTE_TYPE = FID_BOND_BEST_QUOTE_BASE + 105;

const int FID_TRADER_ID_LIST = FID_BOND_BEST_QUOTE_BASE + 106;          // 最优报价，交易员列表
const int FID_BROKER_ID_LIST = FID_BOND_BEST_QUOTE_BASE + 107;          // 最优报价，经纪人列表
const int FID_BID_PRICE_MATCH = FID_BOND_BEST_QUOTE_BASE + 108;	// 最优报价的bid价格的匹配状态,优化paintBestBidPrice
const int FID_OFR_PRICE_MATCH = FID_BOND_BEST_QUOTE_BASE + 109;	// 最优报价的ofr价格的匹配状态,优化paintBestOfrPrice

const int FID_BEST_BID_PRICE_SORT = FID_BOND_BEST_QUOTE_BASE + 110;
const int FID_BEST_OFR_PRICE_SORT = FID_BOND_BEST_QUOTE_BASE + 111;
const int FID_BEST_UNION_BID_VOL_SORT = FID_BOND_BEST_QUOTE_BASE + 112;
const int FID_BEST_UNION_OFR_VOL_SORT = FID_BOND_BEST_QUOTE_BASE + 113;
const int FID_BEST_BID_CLEAR_SPEED = FID_BOND_BEST_QUOTE_BASE + 114;
const int FID_BEST_BID_TRADED_DATE = FID_BOND_BEST_QUOTE_BASE + 115;
const int FID_BEST_BID_SETTLEMENT_DATE = FID_BOND_BEST_QUOTE_BASE + 116;
const int FID_BEST_OFR_CLEAR_SPEED = FID_BOND_BEST_QUOTE_BASE + 117;
const int FID_BEST_OFR_TRADED_DATE = FID_BOND_BEST_QUOTE_BASE + 118;
const int FID_BEST_OFR_SETTLEMENT_DATE = FID_BOND_BEST_QUOTE_BASE + 119;
const int FID_BEST_UNION_BID_CLEAR_SPEED = FID_BOND_BEST_QUOTE_BASE + 120;
const int FID_BEST_UNION_BID_TRADED_DATE = FID_BOND_BEST_QUOTE_BASE + 121;
const int FID_BEST_UNION_BID_SETTLEMENT_DATE = FID_BOND_BEST_QUOTE_BASE + 122;
const int FID_BEST_UNION_OFR_CLEAR_SPEED = FID_BOND_BEST_QUOTE_BASE + 123;
const int FID_BEST_UNION_OFR_TRADED_DATE = FID_BOND_BEST_QUOTE_BASE + 124;
const int FID_BEST_UNION_OFR_SETTLEMENT_DATE = FID_BOND_BEST_QUOTE_BASE + 125;
const int FID_BEST_EXTERN_DEAL_ID = FID_BOND_BEST_QUOTE_BASE + 126;
const int FID_BEST_EXTERN_DEAL_PRICE = FID_BOND_BEST_QUOTE_BASE + 127;
const int FID_BEST_BID_TRADER_ID_LIST = FID_BOND_BEST_QUOTE_BASE + 128;
const int FID_BEST_OFR_TRADER_ID_LIST = FID_BOND_BEST_QUOTE_BASE + 129;
const int FID_BEST_PRICE_CHANGED = FID_BOND_BEST_QUOTE_BASE + 130;
const int FID_BEST_HAVING_BID_DEVIATE = FID_BOND_BEST_QUOTE_BASE + 131;
const int FID_BEST_HAVING_OFR_DEVIATE = FID_BOND_BEST_QUOTE_BASE + 132;
const int FID_BID_BROKER_ID_LIST = FID_BOND_BEST_QUOTE_BASE + 133;
const int FID_BID_EXTERNAL_BROKER_ID_LIST = FID_BOND_BEST_QUOTE_BASE + 134;
const int FID_OFR_BROKER_ID_LIST = FID_BOND_BEST_QUOTE_BASE + 135;

const int FID_BEST_UNION_BID_URGENT = FID_BOND_BEST_QUOTE_BASE + 136;
const int FID_BEST_UNION_OFR_URGENT = FID_BOND_BEST_QUOTE_BASE + 137;
const int FID_BEST_BID_QUOTE_SOURCE = FID_BOND_BEST_QUOTE_BASE + 138;
const int FID_BEST_OFR_QUOTE_SOURCE = FID_BOND_BEST_QUOTE_BASE + 139;
const int FID_BEST_UNION_BID_QUOTE_SOURCE = FID_BOND_BEST_QUOTE_BASE + 140;
const int FID_BEST_UNION_OFR_QUOTE_SOURCE = FID_BOND_BEST_QUOTE_BASE + 141;

//---------------------  Hot Key ----------------------------------
const int FID_HOT_KEY_BASE = 8000;
const int FID_HOT_KEY_ID = FID_HOT_KEY_BASE + 0;          //"HotKeyId": 快捷键ID
const int FID_HOT_KEY_CODE = FID_HOT_KEY_BASE + 1;          //"HotKeyCode":   快捷键Code，如"Ctrl+0"
const int FID_HOT_KEY_FUNCTION_NAME = FID_HOT_KEY_BASE + 2;          //"HotKeyFunctionName": 快捷键关联功能块名称
const int FID_HOT_KEY_DESCRIPTION = FID_HOT_KEY_BASE + 3;          //"HotKeyDescription": 详细信息
const int FID_HOT_KEY_FUNCTION = FID_HOT_KEY_BASE + 4;          //"HotKeyFunction": 快捷键关联功能块代码

//--------------------- Export ------------------------------------
const int FID_EXPORT_BASE = 8500;
const int FID_EXPORT_FILE_PATH = FID_EXPORT_BASE + 0;           //"ExportPath": 服务器返回的保存路径
const int FID_EXPORT_FILE_NAME = FID_EXPORT_BASE + 1;           //"ExportName": 服务器返回的文件名字
const int FID_EXPORT_IS_INTERNAL = FID_EXPORT_BASE + 2;           //"ExportIsInternal": 导出的是否为内部报价 

#define STR(x) #x

#define SET_FID_MAP(fid, column) fid_mp_[fid] = #column;					   //把字符串转成FastDB的SQL语句
#define SET_FID_MAP_COMBIN(fid, column1, column2) fid_mp_[fid] = STR(column1 | column2);

//----------------------System Management--------------------------
const int FID_CONSTANTS_BASE = 9000;
const int FID_CONSTANTS_ID = FID_CONSTANTS_BASE + 0;        //"ConstantsId": 常量id
const int FID_CONSTANTS_NAME = FID_CONSTANTS_BASE + 1;        //"ConstantsName": 常量名称
const int FID_CONSTANTS_VALUE = FID_CONSTANTS_BASE + 2;        //"ConstantsValue": 常量值
const int FID_CONSTANTS_KEY = FID_CONSTANTS_BASE + 3;        //"ConstantsKey": 常量键值

//------------------------Push Message------------------------------
const int FID_PUSH_MESSAGE_BASE = 10000;
const int FID_PUSH_MESSAGE_VERSION = FID_PUSH_MESSAGE_BASE + 0;          //"version"
const int FID_PUSH_MESSAGE_LAST_VERSION = FID_PUSH_MESSAGE_BASE + 1;          //"lastVersion"
const int FID_PUSH_MESSAGE_COMPANY_ID = FID_PUSH_MESSAGE_BASE + 2;          //"companyId"
const int FID_PUSH_MESSAGE_COMPANY_NAME = FID_PUSH_MESSAGE_BASE + 3;          //"companyName"

const int FID_PUSH_MESSAGE_BOND_QUOTE = FID_PUSH_MESSAGE_BASE + 10;         //"bondOfferMessage"            
const int FID_PUSH_MESSAGE_BOND_DEAL = FID_PUSH_MESSAGE_BASE + 11;         //"bondDealMessage"
const int FID_PUSH_MESSAGE_BOND_BEST_QUOTE = FID_PUSH_MESSAGE_BASE + 12;         //"bondBestOfferMessage"
const int FID_PUSH_MESSAGE_BOND_DCS_DEAL = FID_PUSH_MESSAGE_BASE + 13;         //"bondDcsDealMessage"

const int FID_PUSH_MESSAGE_QUOTE_LIST = FID_PUSH_MESSAGE_BASE + 20;         //"bondOffer"
const int FID_PUSH_MESSAGE_DEAL_LIST = FID_PUSH_MESSAGE_BASE + 21;         //"bondDeal"
const int FID_PUSH_MESSAGE_BEST_QUOTE_LIST = FID_PUSH_MESSAGE_BASE + 22;         //"bondBestOffer"
const int FID_PUSH_MESSAGE_DCS_DEAL_LIST = FID_PUSH_MESSAGE_BASE + 23;         //"bondDcsDeal"

const int FID_PUSH_MESSAGE_METHOD_ADD = FID_PUSH_MESSAGE_BASE + 30;         //"methodAdd"
const int FID_PUSH_MESSAGE_METHOD_UPDATE = FID_PUSH_MESSAGE_BASE + 31;         //"methodUpdate"
const int FID_PUSH_MESSAGE_METHOD_DELETE = FID_PUSH_MESSAGE_BASE + 32;         //"methodDelete"
const int FID_PUSH_MESSAGE_METHOD_REFER = FID_PUSH_MESSAGE_BASE + 33;         //"methodRefer"
const int FID_PUSH_MESSAGE_METHOD_UNREFER = FID_PUSH_MESSAGE_BASE + 34;         //"methodUnRefer"

const int FID_PUSH_MODIFY_TIME_QUOTE = FID_PUSH_MESSAGE_BASE + 100;
const int FID_PUSH_MODIFY_TIME_BEST = FID_PUSH_MESSAGE_BASE + 101;
const int FID_PUSH_MODIFY_TIME_DEAL = FID_PUSH_MESSAGE_BASE + 102;
const int FID_PUSH_MODIFY_TIME_REFER = FID_PUSH_MESSAGE_BASE + 103;

//------------------------Stats Message------------------------------
const int FID_STATS_MESSAGE_BASE = 10200;
const int FID_STATS_MESSAGE_DEAL_COUNTS = FID_STATS_MESSAGE_BASE + 0;          //"成交笔数"
const int FID_STATS_MESSAGE_QUOTE_COUNTS = FID_STATS_MESSAGE_BASE + 1;         //"报价次数"
const int FID_STATS_MESSAGE_INSSUER = FID_STATS_MESSAGE_BASE + 2;              //"发行人"
const int FID_STATS_MESSAGE_BANK_ID = FID_STATS_MESSAGE_BASE + 3;
const int FID_STATS_STATUS = FID_STATS_MESSAGE_BASE + 4;						// 状态
const int FID_STATS_TODAY_QUOTE_COUNTS = FID_STATS_MESSAGE_BASE + 5;            // 今日报价数
const int FID_STATS_TODAY_DEAL_COUNTS = FID_STATS_MESSAGE_BASE + 6;             // 今日成交数
const int FID_STATS_TODAY_DEAL_VOLUME = FID_STATS_MESSAGE_BASE + 7;             // 今日成交量
const int FID_STATS_COVER_INST_COUNTS = FID_STATS_MESSAGE_BASE + 8;             // 覆盖机构数
const int FID_STATS_BINDING_INST_COUNTS = FID_STATS_MESSAGE_BASE + 9;           // 绑定机构数
const int FID_STATS_TOTAL_VOLUME = FID_STATS_MESSAGE_BASE + 10;
const int FID_STATS_TIME = FID_STATS_MESSAGE_BASE + 11;

//------------------------Audit Repeat Quote---------------------------
const int FID_AUDIT_BASE = 40000;
const int FID_AUDIT_DIRECTION = FID_AUDIT_BASE + 1;
const int FID_AUDIT_PRICE = FID_AUDIT_BASE + 2;
const int FID_AUDIT_VALUATIONS = FID_AUDIT_BASE + 3;
const int FID_AUDIT_VOLUME = FID_AUDIT_BASE + 4;
const int FID_AUDIT_STATUS = FID_AUDIT_BASE + 5;
const int FID_AUDIT_LINE_ID = FID_AUDIT_BASE + 6;

//------------------------NCD一级---------------------------

//------------------------Undo operate------------------------------
const int FID_UNDO_OPERATE = 20000;
const int FID_UNDO_SERVICE = FID_UNDO_OPERATE + 0;         //not for table field
const int FID_UNDO_METHOD = FID_UNDO_OPERATE + 1;         //not for table field
const int FID_UNDO_METHOD_TYPE = FID_UNDO_OPERATE + 2;
const int FID_UNDO_LIST = FID_UNDO_OPERATE + 3;

//------------------------For QQ2IDB------------------------------
const int FID_QQ2IDB_PARSEDMSG = 30000;
const int FID_QQ2IDB_PARSER_RESULT = FID_QQ2IDB_PARSEDMSG + 1;
const int FID_QQ2IDB_ERROR_MAP = FID_QQ2IDB_PARSEDMSG + 2;
const int FID_QQ2IDB_ERROR_LIST = FID_QQ2IDB_PARSEDMSG + 3;

//------------------------For QQ2IDB ERROR MSG------------------------------
const int FID_QQ2IDB_IS_SUCCESS = 31000;                     // bool, create quote success or not
const int FID_QQ2IDB_ERROR_CODE = FID_QQ2IDB_IS_SUCCESS + 1;  // pre-defined error code
const int FID_QQ2IDB_ERROR_MSG = FID_QQ2IDB_IS_SUCCESS + 2;  // error msg

//---------------------------------- USER SETTINGS----------------------------------------
const int FID_USER_SETTINGS_BASE = 35000;
const int FID_USER_SETTINGS_TYPE = FID_USER_SETTINGS_BASE + 1;
const int FID_USER_SETTINGS_STATUS = FID_USER_SETTINGS_BASE + 2;
const int FID_USER_SETTINGS_VALUE = FID_USER_SETTINGS_BASE + 3;

const int FID_ACK_MSG_HEAD         = 7777;
const int FID_ACK_MSG_BODY         = 7778;
const int FID_BOND_CALCULATOR         = 9898;

//------------------------NCD一级---------------------------
const int FID_NCDP_BASE = 50000;

const int FID_NCDP_ADD_LIST = FID_NCDP_BASE + 1;                       // 新增列表
const int FID_NCDP_UPDATE_LIST = FID_NCDP_BASE + 2;                    // 更新列表
const int FID_NCDP_DELETE_LIST = FID_NCDP_BASE + 3;                    // 删除列表

const int FID_NCDP_JMSREPLY_TO = FID_NCDP_BASE + 100;                    // 补差应答地址
const int FID_NCDP_BEGIN_ID = FID_NCDP_BASE + 101;                       // 补差开始ID
const int FID_NCDP_END_ID = FID_NCDP_BASE + 102;                         // 补差结束ID

// 定制券(发行机构、期限、发行日期、利率类型联合主键)
const int FID_NCDPLS_BASE = FID_NCDP_BASE + 1000;
const int FID_NCDPLS_ISSUE_DEPT_ID = FID_NCDPLS_BASE + 1;                // 发行机构ID
const int FID_NCDPLS_ISSUE_DEPT_CODE = FID_NCDPLS_BASE + 2;              // 发行机构code
const int FID_NCDPLS_ISSUE_DEPT_NAME = FID_NCDPLS_BASE + 3;              // 发行机构名字
const int FID_NCDPLS_QUOTE_DEPT_ID = FID_INSTITUTION_ID;                 // 报价机构ID
const int FID_NCDPLS_QUOTE_DEPT_NAME = FID_INSTITUTION_NAME;             // 报价机构名字
const int FID_NCDPLS_QUOTE_DEPT_NCD_NAME = FID_NCDPLS_BASE + 4;          // 报价机构NCD简称
const int FID_NCDPLS_GOODS_LEVEL = FID_ISSUER_RATING_FILTER;              // 报价评级(AAA、AA+、AA-)
const int FID_NCDPLS_GOODS_LEVEL_ORDER = FID_ISSUER_RATING_ORDER;         // 评级排序
const int FID_NCDPLS_TERMINAL = FID_TIME_TO_MATURITY_REAL;                // 期限(1M)
const int FID_NCDPLS_ISSUE_PRICE = FID_NCDPLS_BASE + 8;                   // 发行价格
const int FID_NCDPLS_ISSUE_PRICE_FLAG = FID_NCDPLS_BASE + 9;              // 发行价格标志(0：正常，1：推荐)
const int FID_NCDPLS_ISSUE_PRICE_SORT = FID_NCDPLS_BASE + 10;              // 发行价格用于排序
const int FID_NCDPLS_INTEREST_TYPE = FID_NCDPLS_BASE + 11;                 // issue_price字段标识(固息，Shibor)
const int FID_NCDPLS_PLANED_ISSUE_AMOUNT = FID_NCDPLS_BASE + 12;           // 发行数量
const int FID_NCDPLS_ACTUAL_ISSUE_AMOUNT = FID_NCDPLS_BASE + 13;          // 实际发行数量
const int FID_NCDPLS_ISSUE_DAY = FID_NCDPLS_BASE + 14;                    // 发行日<星期> 周一~周日、今日
const int FID_NCDPLS_ISSUE_DATE = FID_NCDPLS_BASE + 15;                   // 发行日期(2018-01-22)
const int FID_NCDPLS_REMARK = FID_NCDPLS_BASE + 16;                       // 备注
const int FID_NCDPLS_GOODS_STATUS = FID_NCDPLS_BASE + 17;                 // 一级券状态（0正常,1Reffer,2删除）
const int FID_NCDPLS_MODIFY_TIME = FID_NCDPLS_BASE + 18;                  // 修改时间
const int FID_NCDPLS_PRICE_CHANGE = FID_NCDPLS_BASE + 19;                 // 价格变动
const int FID_NCDPLS_ENTERPRISE_TYPE = FID_NCDPLS_BASE + 20;              // 机构类型（SPB: 政策性;SHB: 股份制;MCB: 大行;CCB: 城商行;FRB: 固定利率;OTB: 其他债券;RRB、RTB: 其他 ）
const int FID_NCDPLS_BROKER_ID = FID_BROKER_ID;                           // 经纪人ID
const int FID_NCDPLS_BROKER_NAME = FID_NCDPLS_BASE + 22;                  // 经纪人名称
const int FID_NCDPLS_DESCRIPTION = FID_NCDPLS_BASE + 23;                  // 询满或者空
const int FID_NCDPLS_FLAG_VIP = FID_NCDPLS_BASE + 24;                     // VIP付费机构标识（0普通机构，1付费机构)
const int FID_NCDPLS_OFFER_ID = FID_NCDPLS_BASE + 25;                     // offer id，入库流水ID
const int FID_NCDPLS_PLANED_ISSUE_AMOUNT_SORT = FID_NCDPLS_BASE + 26;     // 计划发行数量排序
const int FID_NCDPLS_STATUS = FID_NCDPLS_BASE + 27;                       // 状态（1：有效，2：无效）
const int FID_NCDPLS_CREATE_TIME = FID_NCDPLS_BASE + 28;                  // 创建时间

// 投标单
const int FID_NCDPDEAL_BASE = FID_NCDP_BASE + 2000;
const int FID_NCDPDEAL_BID_BANK_ID = FID_NCDPDEAL_BASE + 1;                 // 买方机构ID
const int FID_NCDPDEAL_BID_BANK_CODE = FID_NCDPDEAL_BASE + 2;               // 买方机构代码
const int FID_NCDPDEAL_BID_BANK_CITY = FID_NCDPDEAL_BASE + 3;               // 买方机构城市
const int FID_NCDPDEAL_BID_AGENT_ID = FID_NCDPDEAL_BASE + 4;                // 买方交易员ID
const int FID_NCDPDEAL_BID_AGENT_CODE = FID_NCDPDEAL_BASE + 5;              // 买方交易员代码
const int FID_NCDPDEAL_BID_BROKER_ID = FID_NCDPDEAL_BASE + 6;               // 买方经纪人代码
const int FID_NCDPDEAL_BID_BROKER_NAME = FID_NCDPDEAL_BASE + 7;             // 买方经纪人名字
const int FID_NCDPDEAL_BID_CHECK = FID_NCDPDEAL_BASE + 8;                   // 买方check
const int FID_NCDPDEAL_OFR_BANK_ID = FID_NCDPDEAL_BASE + 9;                 // 卖方机构ID
const int FID_NCDPDEAL_OFR_BANK_CODE = FID_NCDPDEAL_BASE + 10;              // 卖方机构代码
const int FID_NCDPDEAL_OFR_BANK_CITY = FID_NCDPDEAL_BASE + 11;              // 卖方机构城市
const int FID_NCDPDEAL_OFR_AGENT_ID = FID_NCDPDEAL_BASE + 12;               // 卖方交易员ID
const int FID_NCDPDEAL_OFR_AGENT_CODE = FID_NCDPDEAL_BASE + 13;             // 卖方交易员代码
const int FID_NCDPDEAL_OFR_BROKER_ID = FID_NCDPDEAL_BASE + 14;              // 卖方经纪人ID
const int FID_NCDPDEAL_OFR_BROKER_NAME = FID_NCDPDEAL_BASE + 15;            // 卖方经纪人名字
const int FID_NCDPDEAL_OFR_CHECK = FID_NCDPDEAL_BASE + 16;                  // 卖方check
const int FID_NCDPDEAL_PRICE = FID_NCDPDEAL_BASE + 17;                      // 价格
const int FID_NCDPDEAL_VOLUME = FID_NCDPDEAL_BASE + 18;                     // 量
const int FID_NCDPDEAL_CURRENCY = FID_NCDPDEAL_BASE + 19;                   // 佣金（如：CNY）
const int FID_NCDPDEAL_DELIVERY_DATE = FID_NCDPDEAL_BASE + 20;              // 交割日
const int FID_NCDPDEAL_MODIFY_TIME = FID_NCDPDEAL_BASE + 21;                // 修改时间

#endif // _FID_H_
