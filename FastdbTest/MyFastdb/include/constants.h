#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <string>
#include <vector>
#include "sdbus/string.h" 
#include "sdbus/message.h"

#define DECIMAL_NULL -999
#define DOUBLE_NULL -999

// service name
static const std::string kServiceLogin = "Login";
static const std::string kServiceHeartBeat = "HeartBeat";
static const std::string kServiceBondInfo = "Bond";
static const std::string kServiceBondQuote = "BondQuote";
static const std::string kServiceBondDeal = "BondDeal";
static const std::string kServiceDcsBondDeal = "DcsBondDeal";
static const std::string kServiceBondBestQuote = "BondBestQuote";
static const std::string kServiceBondCompletedQuote = "BondCompletedQuote";
static const std::string kServiceManagement = "Management";
static const std::string kServiceHotKey = "HotKey";
static const std::string kServiceProductFavor = "ProductFavor";
static const std::string kServiceSync = "Sync";
static const std::string kServiceCalculator = "Calculator";
static const std::string kServiceQBSync = "QBSync";
static const std::string kServiceIDCSync = "IDCSync";
static const std::string kServiceUserInfo = "UserInfo";
static const std::string kServiceInit = "Init";
static const std::string kServiceVersion = "Version";
static const std::string kServiceCRM = "CRM";
static const std::string kServiceDCS = "DCS";
static const std::string kServiceIDC = "IDC";
static const std::string kServiceTransfer = "Transfer";
static const std::string kServiceTraderPreference = "TraderPreference";
static const std::string kServiceMemCache = "MemCache";

// qpid service queue
static const std::string kServiceQueueLogin = "CQ.SDBUS.Bond.Service.Login";
static const std::string kServiceQueueHeartBeat = "CQ.SDBUS.Bond.Service.HeartBeat";
static const std::string kServiceQueueBondInfo = "CQ.SDBUS.Bond.Service.BondInfo";
static const std::string kServiceQueueBondQuote = "CQ.SDBUS.Bond.Service.BondQuote";
static const std::string kServiceQueueBondDeal = "CQ.SDBUS.Bond.Service.BondDeal";
static const std::string kServiceQueueDcsBondDeal = "CQ.SDBUS.Bond.Service.DcsBondDeal";
static const std::string kServiceQueueBondBestQuote = "CQ.SDBUS.Bond.Service.BondBestQuote";
static const std::string kServiceQueueBondCompletedQuote = "CQ.SDBUS.Bond.Service.BondCompletedQuote";
static const std::string kServiceQueueManagement = "CQ.SDBUS.Bond.Service.Management";
static const std::string kServiceQueueHotKey = "CQ.SDBUS.Bond.Service.HotKey";
static const std::string kServiceQueueProductFavor = "CQ.SDBUS.Bond.Service.ProductFavor";
static const std::string kServiceQueueSync = "CQ.SDBUS.Bond.Service.Sync";
static const std::string kServiceQueueBasicSync = "CQ.SDBUS.Basic.Service.Sync";
static const std::string kServiceQueueMessageResend = "CQ.SDBUS.Bond.Service.MessageResend";
static const std::string kServiceQueueMessageResendRecieve = "CQ.SDBUS.Bond.Service.MessageResendRecieve";
static const std::string kServiceQueueCalculator = "CQ.SDBUS.Bond.Service.Calculator";
static const std::string kServiceQueueUserInfo = "CQ.SDBUS.Bond.Service.UserInfo";
static const std::string kServiceQueuePinyin = "CQ.SDBUS.Bond.Service.Pinyin";
static const std::string kServiceQueueQQ2IDB = "CQ.SDBUS.Bond.Service.QQ2IDBS";
static const std::string kServiceQueueInit = "CQ.SDBUS.Bond.Service.Init";
static const std::string kServiceQueueIDC = "CQ.SDBUS.Bond.Service.IDC";
static const std::string kServiceQueueTraderPreference = "CQ.SDBUS.Bond.Service.TraderPreference";
static const std::string kServiceQueueMemCache = "CQ.SDBUS.Bond.Service.MemCache";

// qpid topic
static const std::string kTopicLogin = "CQ.SDBUS.Bond.Topic.Login";
static const std::string kTopicHeartBeat = "CQ.SDBUS.Bond.Topic.HeartBeat";
static const std::string kTopicBondInfo = "CQ.SDBUS.Bond.Topic.BondInfo";
static const std::string kTopicBondDetail = "CQ.SDBUS.Bond.Topic.BondDetail";
static const std::string kTopicBondQuote = "CQ.SDBUS.Bond.Topic.BondQuote";
static const std::string kTopicBondDeal = "CQ.SDBUS.Bond.Topic.BondDeal";
static const std::string kTopicDcsBondDeal = "CQ.SDBUS.Bond.Topic.DcsBondDeal";
static const std::string kTopicBondBestQuote = "CQ.SDBUS.Bond.Topic.BondBestQuote";
static const std::string kTopicBondCompletedQuote = "CQ.SDBUS.Bond.Topic.BondCompletedQuote";
static const std::string kTopicManagement = "CQ.SDBUS.Bond.Topic.Management";
static const std::string kTopicHotKey = "CQ.SDBUS.Bond.Topic.HotKey";
static const std::string kTopicProductFavor = "CQ.SDBUS.Bond.Topic.ProductFavor";
static const std::string kTopicSync = "CQ.SDBUS.Bond.Topic.Sync";
static const std::string kTopicBasicSync = "CQ.SDBUS.Basic.Topic.Sync";
static const std::string kTopicIDC = "CQ.SDBUS.Bond.Topic.IDC";
static const std::string kTopicStatsAgentBond = "CQ.SDBUS.Bond.Topic.StatsAgentBond";
static const std::string kTopicStatsAgentWhitelist = "CQ.SDBUS.Bond.Topic.StatsAgentWhitelist";
static const std::string kTopicMemCache = "CQ.SDBUS.Bond.Topic.MemCache";

// qpid send service ret queue
static const std::string kServiceRetQueueMain = "CQ.SDBUS.Bond.RetQueue.Main";
static const std::string kServiceRetQueueProductFavor = "CQ.SDBUS.Bond.RetQueue.ProductFavor";
static const std::string kServiceRetQueueHotKey = "CQ.SDBUS.Bond.RetQueue.HotKey";
static const std::string kServiceRetQueueBasicData = "CQ.SDBUS.Bond.RetQueue.BasicData";

// qpid for QB sync
extern std::string kQBSyncTopic;
extern std::string kQBSyncServiceQueue;

// qpid for IDC sync
extern std::string kIDCSyncTopic;
extern std::string kIDCSyncServiceQueue;

// best quote make and insert into to mysql database
extern std::string kCalcAndInsertIntoMysqlDatabase;
extern bool kFirstLaunch;

// use cache in mem
extern bool useCacheInMem;

extern bool kCacheLoaded; // is cache loaded
extern time_t kServiceReadyTime;
extern int kCompanySize;

extern bool kRunningSync; // 是否正在运行同步任务

extern std::string kSchemaAccount;
extern std::string kSchemaBond;
extern std::string kSchemaBase;
// database pool config
static const int kDBCoonPoolSize = 5;

// 报价类型
const std::string kQuoteTypeYield = "3"; // 收益率
const std::string kQuoteTypeCleanPrice = "1"; // 净价
const std::string kQuoteTypeDirtyPrice = "2"; // 全价
const std::string kQuoteTypeSpread = "4"; // 利差
const std::string kQuoteTypeFandian = "8"; // 返点

// 成交单确认状态
const std::string kDealStatusNoChecked = "0";
const std::string kDealStatusOneChecked = "1";
const std::string kDealStatusBothChecked = "2";
const std::string kDealStatusCompleted = "3";
const std::string kDealStatusRefered = "4";
const std::string kDealStatusCanceled = "5";
const std::string kDealNothingDone = "8";

// DCS成交单状态
const std::string kDcsDealtoBeConfirm = "0"; // 待确认
const std::string kDcsDealtoBeSubmit = "1"; // 待提交
const std::string kDcsDealSubmitted = "2"; // 已提交
const std::string kDcsDealPassed = "3"; // 已通过
const std::string kDcsDealNoPass = "5"; // 未通过
const std::string kDcsDealDestroyed = "7"; // 已毁单
const std::string kDcsDealIntheReview = "13"; // 送审中（高级经理签字状态）
const std::string kDcsDealDeleted = "-1"; // 已删除

const std::string kDcsDealtoBeConfirmString = "待确认";
const std::string kDcsDealtoBeSubmitString = "待提交";
const std::string kDcsDealSubmittedString = "已提交";
const std::string kDcsDealPassedString = "已通过";
const std::string kDcsDealNoPassString = "未通过";
const std::string kDcsDealDestroyedString = "已毁单";
const std::string kDcsDealIntheReviewString = "送审中";
const std::string kDcsDealDeletedString = "已删除";
const std::string kDcsDealInHandString = "处理中";

//DCS开关
extern bool kDcsEnable;

//CRM开关
extern bool kCrmEnable;

//DCS收单状态
extern bool kDcsSwitchStatus;

//每次请求DCS成交单数据的条数
extern int kDcsPageSize;

extern bool kDcsAsyncInitEnable; // 是否开启Dcs异步加载，开启时，系统启动时只加载近一周的数据，只启动时生效，凌晨reset时不使用该参数

const int kInitVersion = 0;

const std::string kCompanyTP = "1";
const std::string kCompanyICAP = "2";
const std::string kCompanyBGC = "3";
const std::string kCompanyPATR = "4";
const std::string kCompanyTJXT = "5";   //信唐

const std::string kOperatorAdd = "Add";
const std::string kOperatorUpdate = "Update";
const std::string kOperatorDelete = "Delete";

const std::string kPublishAdd = "AddList";
const std::string kPublishUpdate = "UpdateList";
const std::string kPublishDelete = "DeleteList";

const int kNormalQuote = 1;   //正常导出
const int kInternalQuote = 2; //暗盘导出
const std::string kCompanyTPName = "TPSC";
const std::string kCompanyICAPName = "上海国际货币";

extern std::string GetCurrentCompanyId(const sdbus::Message* msg);
extern std::string GetCurrentCompanyId(const std::string& account_id);
extern std::string GetCurrentCompanyName(const std::string& company_id);
extern std::string GetCurrentCompanyIdByAccountName(const std::string& account_name);
extern std::string GetCurrentAccountName(const std::string& account_id);

// service mode
extern void SetMasterMode();
extern void SetSlaveMode();
extern bool SingleMode();
extern bool MasterMode();
extern bool SlaveMode();

// Bid/Ofr
const int kBidQuote = 1;
const int kOfrQuote = -1;

// 内部报价
const std::string kQuoteInternal = "2"; // 内部报价
const std::string kQuoteNotInternal = "1"; // 非内部报价

// 内部成交
const std::string kDealInternal = "2"; // 内部成交
const std::string kDealNotInternal = "1"; // 非内部成交

const std::string kBestQuoteSettingExternal = "0";
const std::string kBestQuoteSettingInternal = "1";
extern std::string kBestQuoteSetting; // 0-暗盘不参与最优 1-暗盘参与最优

// 上市日期类型 <与客户端值同步>
const std::string kMaturityDateNormal = "Normal";
const std::string kMaturityDateHoliday = "Holiday";
const std::string kMaturityDateSaturday = "Saturday";
const std::string kMaturityDateSunday = "Sunday";

// 条数限制为1500
static const int kAmountLimit = 1500;

extern int kServiceSyncLimit;
extern int kServiceSyncTimeout;
extern int kDBBatchCommit;
// fastdb in条件查询时，最大1000条
static const int kSqlMax = 1000;

// All method types
const std::string mBond_AccountInfo = "Bond.AccountInfo";
const std::string mBond_BondDeviatedValue = "Bond.BondDeviatedValue";
const std::string mBond_BondValuation = "Bond.BondValuation";
const std::string mBond_BrokerBindingTrader = "Bond.BrokerBindingTrader";
const std::string mBond_GetBondInfoByCondition = "Bond.GetBondInfoByCondition";
const std::string mBond_InstitutionInfo = "Bond.InstitutionInfo";
const std::string mBond_TraderInfo = "Bond.TraderInfo";
const std::string mBondBestQuote_BestQuoteList = "BondBestQuote.BestQuoteList";
const std::string mBondBestQuote_CompletedQuoteList = "BondBestQuote.CompletedQuoteList";
const std::string mBondCompletedQuote_CompletedQuoteDetailList = "BondCompletedQuote.CompletedQuoteDetailList";
const std::string mBondDeal_DealList = "BondDeal.DealList";
const std::string mDcsBondDeal_DealList = "DcsBondDeal.DealList";
const std::string mDcsBondDeal_DcsFailMsg = "DcsBondDeal.DcsFailMsg";
const std::string mBondDetail_BondSmallViewDetail = "BondDetail.BondSmallViewDetail";
const std::string mBondInfo_BondSmallView = "BondInfo.BondSmallView";
const std::string mBondQuote_QuoteList = "BondQuote.QuoteList";
const std::string mBondQuote_ReferQuoteList = "BondQuote.ReferQuoteList";
const std::string mBond_CacheReload = "Bond.CacheReload";
const std::string mBond_PublishQB = "Bond.PublishQB";
const std::string mBond_PublishIDC = "Bond.PublishIDC";
const std::string mTraderPreference_StatsAgentBond = "TraderPreference.StatsAgentBond";
const std::string mTraderPreference_StatsAgentWhitelist = "TraderPreference.StatsAgentWhitelist";
const std::string mMemCache_BrokerTodayProfile = "MemCache.BrokerTodayProfile";

//user settings 
static const int USERSETTINGTYPE_COPYSET = 0;
static const int USERSETTINGTYPE_REMINDERSET = 1;

static const std::string USERSETTINGS_COPY_PY = "copy_py"; // 含久期
static const std::string USERSETTINGS_COPY_VALUATIONS = "copy_valuations"; // 含估值
static const std::string USERSETTINGS_REMIND_FIRSTQUOTE = "first_quote";
static const std::string USERSETTINGS_REMIND_BETTERQUOTE = "better_quote";
static const std::string USERSETTINGS_REMIND_BESTQUOTE_WIDE = "bestquote_wide";
static const std::string USERSETTINGS_REMIND_BESTQUOTE_NARROW = "bestquote_narrow";
static const std::string USERSETTINGS_REMIND_BESTQUOTE_NARROW_BP = "bestquote_narrow_bp";
static const std::string USERSETTINGS_REMIND_DEAL = "deal";
static const std::string USERSETTINGS_REMIND_POPWINDOW = "pop_window";

const std::string COMPANY_BOND_PUBLIC = "_PUBLIC_";
const std::string COMPANY_BOND_PRIVATE = "_PRIVATE_";

const std::string OPER_SOURCE_DEFAULT = "1"; // bond client
const std::string OPER_SOURCE_IDC = "2"; // idc server

const std::string OPER_TYPE_BROKER = "1"; // offline, broker
const std::string OPER_TYPE_TRADER = "2"; // online, trader

const int SERVICE_STATUS_INIT = 0;
const int SERVICE_STATUS_READY = 1;
const int SERVICE_STATUS_STOP = 2;

const std::vector<std::string> BOND_CATEGORYS = { "BCO", "BNC", "ABS", "NCD" };

#endif

