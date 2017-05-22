package se.liu.ida.tdp024.account.data.api.facade;

import se.liu.ida.tdp024.account.data.api.entity.Account;
import java.util.List;

public interface AccountEntityFacade {
    public long createAccount(String personKey, String bankKey, String accountType);
    
    public List<Account> findAccountsFor(String personKey);
    
    //public boolean hasEnoughHoldings(long id, long amount);
    
    //public boolean canFindAccount(long id);
    
    //public boolean changeAccountHoldings(long accountID, long amount, String type);
    
    //public Account getAccount(long id);
}
