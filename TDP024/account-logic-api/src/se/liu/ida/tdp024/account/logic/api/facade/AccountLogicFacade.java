package se.liu.ida.tdp024.account.logic.api.facade;

import java.util.List;
import se.liu.ida.tdp024.account.data.api.entity.Account;


public interface AccountLogicFacade {
    public long createAccount(String personName, String bankName, String accountType);
    
    public List<Account> findAccountsFor(String personName);
}
