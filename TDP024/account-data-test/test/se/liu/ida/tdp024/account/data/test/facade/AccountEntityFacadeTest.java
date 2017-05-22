package se.liu.ida.tdp024.account.data.test.facade;

import java.util.List;
import org.junit.After;
import org.junit.Assert;
import org.junit.Test;
import se.liu.ida.tdp024.account.data.api.facade.AccountEntityFacade;
import se.liu.ida.tdp024.account.data.api.util.StorageFacade;

import se.liu.ida.tdp024.account.data.impl.db.facade.AccountEntityFacadeDB;
import se.liu.ida.tdp024.account.data.impl.db.util.StorageFacadeDB;
import se.liu.ida.tdp024.account.data.api.entity.Account;

public class AccountEntityFacadeTest {
    
    //---- Unit under test ----//
    private final AccountEntityFacade accountEntityFacade = new AccountEntityFacadeDB();
    private final StorageFacade storageFacade = new StorageFacadeDB();
    
    @After
    public void tearDown() {
        storageFacade.emptyStorage();
    }
    
    @Test
    public void testCreate() {
        // Create CHECK account
        long id = accountEntityFacade.createAccount("testCreatePersonKey", "testCreateBankKey", "CHECK");
        Assert.assertFalse(id == -1);
        // Create SAVINGS account
        id = accountEntityFacade.createAccount("testCreatePersonKey", "testCreateBankKey", "SAVINGS");
        Assert.assertFalse(id == -1);
        // Create BANANER account (should fail)
        id = accountEntityFacade.createAccount("testCreatePersonKey", "testCreateBankKey", "BANANER");
        Assert.assertTrue(id == -1);
    }
    
    @Test
    public void testFindAllAccountsFor() {
        accountEntityFacade.createAccount("testFindAllAccountsPersonKey", "testFindAllAccountsBankKey", "CHECK");
        accountEntityFacade.createAccount("testFindAllAccountsPersonKey", "testFindAllAccountsBankKey", "SAVINGS");
        accountEntityFacade.createAccount("testFindAllAccountsPersonKey", "testFindAllAccountsBankKey2", "CHECK");
        // Below should not be created
        accountEntityFacade.createAccount("testFindAllAccountsPersonKey", "testFindAllAccountsBankKey2", "BANANER");
        
        {
            List<Account> accs = accountEntityFacade.findAccountsFor("testFindAllAccountsPersonKey");
            Assert.assertNotNull(accs);
            Assert.assertEquals(3, accs.size());
            for (Account account : accs) {
                Assert.assertTrue(account.getPersonKey().equals("testFindAllAccountsPersonKey"));
            }
        }
        {
            List<Account> accs = accountEntityFacade.findAccountsFor("Banannyckeln");
            Assert.assertNotNull(accs);
            Assert.assertEquals(0, accs.size());
        }
    }
}