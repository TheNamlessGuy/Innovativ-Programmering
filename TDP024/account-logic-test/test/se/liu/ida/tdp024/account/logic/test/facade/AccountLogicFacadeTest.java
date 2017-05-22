package se.liu.ida.tdp024.account.logic.test.facade;

import java.util.List;
import junit.framework.Assert;
import org.junit.After;
import org.junit.Test;
import se.liu.ida.tdp024.account.data.api.entity.Account;
import se.liu.ida.tdp024.account.data.api.util.StorageFacade;
import se.liu.ida.tdp024.account.data.impl.db.facade.AccountEntityFacadeDB;
import se.liu.ida.tdp024.account.data.impl.db.util.StorageFacadeDB;
import se.liu.ida.tdp024.account.logic.api.facade.AccountLogicFacade;
import se.liu.ida.tdp024.account.logic.impl.facade.AccountLogicFacadeImpl;

public class AccountLogicFacadeTest {

    
    //--- Unit under test ---//
    public AccountLogicFacade accountLogicFacade = new AccountLogicFacadeImpl(new AccountEntityFacadeDB());
    
    public StorageFacade storageFacade = new StorageFacadeDB();
    
    @After
    public void tearDown() {
        storageFacade.emptyStorage();
    }
    
    @Test
    public void testCreate() {
        // Create a savings account
        long id = accountLogicFacade.createAccount("coolmarkus", "tjuvbank", "SAVINGS");
        Assert.assertTrue(id != -1);
        // Create a check account
        id = accountLogicFacade.createAccount("coolmarkus", "tjuvbank", "CHECK");
        Assert.assertTrue(id != -1);
        
        // Can create the "same" account twice
        id = accountLogicFacade.createAccount("coolmarkus", "tjuvbank", "SAVINGS");
        Assert.assertTrue(id != -1);
        
        // No name
        id = accountLogicFacade.createAccount("", "tjuvbank", "SAVINGS");
        Assert.assertTrue(id == -1);
        // No bank
        id = accountLogicFacade.createAccount("coolmarkus", "", "SAVINGS");
        Assert.assertTrue(id == -1);
        // No accounttype
        id = accountLogicFacade.createAccount("coolmarkus", "tjuvbank", "");
        Assert.assertTrue(id == -1);
        
        // Wrong name
        id = accountLogicFacade.createAccount("tjabba", "tjuvbank", "SAVINGS");
        Assert.assertTrue(id == -1);
        // Wrong bank name
        id = accountLogicFacade.createAccount("coolmarkus", ":(", "SAVINGS");
        Assert.assertTrue(id == -1);
        // Wrong accounttype
        id = accountLogicFacade.createAccount("coolmarkus", "tjuvbank", "BANANER");
        Assert.assertTrue(id == -1);
        
        // Null name
        id = accountLogicFacade.createAccount(null, "tjuvbank", "SAVINGS");
        Assert.assertTrue(id == -1);
        // Null bank
        id = accountLogicFacade.createAccount("coolmarkus", null, "SAVINGS");
        Assert.assertTrue(id == -1);
        // Null accounttype
        id = accountLogicFacade.createAccount("coolmarkus", "tjuvbank", null);
        Assert.assertTrue(id == -1);
    }
    
    @Test
    public void testFindAll()
    {
        accountLogicFacade.createAccount("coolmarkus", "tjuvbank", "SAVINGS");
        accountLogicFacade.createAccount("coolmarkus", "tjuvbank", "CHECK");
        accountLogicFacade.createAccount("coolmarkus", "SWEDBANK", "SAVINGS");
        accountLogicFacade.createAccount("coolmarkus", "SBAB", "SAVINGS");
        accountLogicFacade.createAccount("Q", "IKANOBANKEN", "SAVINGS");
        
        List<Account> accs = accountLogicFacade.findAccountsFor("coolmarkus");
        Assert.assertEquals(4, accs.size());
        
        accs = accountLogicFacade.findAccountsFor(null);
        Assert.assertEquals(0, accs.size());
        
        accs = accountLogicFacade.findAccountsFor("coolare markus");
        Assert.assertEquals(0, accs.size());
        
        accs = accountLogicFacade.findAccountsFor("");
        Assert.assertEquals(0, accs.size());
    }
}