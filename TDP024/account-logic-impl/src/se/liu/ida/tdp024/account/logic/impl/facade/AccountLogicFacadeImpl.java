package se.liu.ida.tdp024.account.logic.impl.facade;

import java.util.ArrayList;
import java.util.List;
import se.liu.ida.tdp024.account.data.api.entity.Account;
import se.liu.ida.tdp024.account.data.api.facade.AccountEntityFacade;
import se.liu.ida.tdp024.account.logic.api.facade.AccountLogicFacade;
import se.liu.ida.tdp024.account.util.http.HTTPHelper;
import se.liu.ida.tdp024.account.util.http.HTTPHelperImpl;
import se.liu.ida.tdp024.account.util.json.AccountJsonSerializer;
import se.liu.ida.tdp024.account.util.json.AccountJsonSerializerImpl;
import se.liu.ida.tdp024.account.util.logger.Logger;
import se.liu.ida.tdp024.account.util.logger.Logger.LoggerLevel;
import se.liu.ida.tdp024.account.util.logger.LoggerImpl;

class Response {
    public String name;
    public String key;
}

public class AccountLogicFacadeImpl implements AccountLogicFacade {
    
    final private AccountEntityFacade accountEntityFacade;
    final private Logger logger;
    final private HTTPHelper http;    
    final private AccountJsonSerializer jsoner;
    
    static final private String personAddress = "http://localhost:8061/person/";
    static final private String bankAddress = "http://localhost:8071/bank/";
    
    public AccountLogicFacadeImpl(AccountEntityFacade accountEntityFacade) {
        this.accountEntityFacade = accountEntityFacade;
        logger = new LoggerImpl();
        http = new HTTPHelperImpl();
        jsoner = new AccountJsonSerializerImpl();
    }
    
    @Override
    public long createAccount(String personName, String bankName, String accountType)
    {
        logger.log(LoggerLevel.INFO, "createAccount(" + personName + ", " + bankName + ", " + accountType + ")");
        
        if (personName == null || bankName == null || accountType == null) {
            logger.log(LoggerLevel.INFO, "createAccount(" + personName + ", " + bankName + ", " + accountType + ") => One of the parameters was not defined");
            return -1;
        }
        
        String response = http.get(personAddress + "find.name/", "name", personName);
        if (response.equals("null")) {
            logger.log(LoggerLevel.INFO, "createAccount(" + personName + ", " + bankName + ", " + accountType + ") => Person not found");
            return -1;
        }
        Response person = jsoner.fromJson(response, Response.class);
        logger.log(LoggerLevel.INFO, "createAccount(" + personName + ", " + bankName + ", " + accountType + ") => Person gotten (key: " + person.key + ")");
        
        response = http.get(bankAddress + "find.name/", "name", bankName);
        if (response.equals("null")) {
            logger.log(LoggerLevel.INFO, "createAccount(" + personName + ", " + bankName + ", " + accountType + ") => Bank not found");
            return -1;
        }
        Response bank = jsoner.fromJson(response, Response.class);
        logger.log(LoggerLevel.INFO, "createAccount(" + personName + ", " + bankName + ", " + accountType + ") => Bank gotten (key: " + bank.key + ")");
        
        return accountEntityFacade.createAccount(person.key, bank.key, accountType);
    }
    
    @Override
    public List<Account> findAccountsFor(String personName)
    {
        logger.log(LoggerLevel.INFO, "findAccountsFor(" + personName + ")");
        String response = http.get(personAddress + "find.name/", "name", personName);
        if (response.equals("null")) {
            logger.log(LoggerLevel.INFO, "findAccountsFor(" + personName + ") => Person not found");
            return new ArrayList<Account>();
        }
        Response person = jsoner.fromJson(response, Response.class);
        logger.log(LoggerLevel.INFO, "findAccountsFor(" + personName + ") => Person gotten (key: " + person.key + ")");
        
        return accountEntityFacade.findAccountsFor(person.key);
    }    
}
