package se.liu.ida.tdp024.account.data.test.facade;

import java.util.List;
import org.junit.After;
import org.junit.Assert;
import org.junit.Test;
import se.liu.ida.tdp024.account.data.api.entity.Transaction;
import se.liu.ida.tdp024.account.data.api.facade.TransactionEntityFacade;
import se.liu.ida.tdp024.account.data.api.facade.AccountEntityFacade;
import se.liu.ida.tdp024.account.data.api.util.StorageFacade;

import se.liu.ida.tdp024.account.data.impl.db.facade.TransactionEntityFacadeDB;
import se.liu.ida.tdp024.account.data.impl.db.facade.AccountEntityFacadeDB;
import se.liu.ida.tdp024.account.data.impl.db.util.StorageFacadeDB;

public class TransactionEntityFacadeTest {
    
    //---- Unit under test ----//
    private final TransactionEntityFacade transactionEntityFacade = new TransactionEntityFacadeDB();
    private final AccountEntityFacade accountEntityFacade = new AccountEntityFacadeDB();
    private final StorageFacade storageFacade = new StorageFacadeDB();
    
    @After
    public void tearDown() {
        storageFacade.emptyStorage();
    }
    
    @Test
    public void testCreate() {
        long id = accountEntityFacade.createAccount("testCreatePersonKey", "testCreateBankKey", "CHECK");
        // Add 100 to account
        long tid = transactionEntityFacade.createTransaction(id, 100, "CREDIT");
        Assert.assertTrue(tid != -1);
        // Remove 100 from account
        tid = transactionEntityFacade.createTransaction(id, 100, "DEBIT");
        Assert.assertTrue(tid != -1);
        // Remove 100 from account (should go through but get FAILED status)
        tid = transactionEntityFacade.createTransaction(id, 100, "DEBIT");
        Assert.assertTrue(tid != -1);
        // Random id
        tid = transactionEntityFacade.createTransaction(500, 0, "CREDIT");
        Assert.assertTrue(tid == -1);
        // Negative value (should go through but get FAILED status)
        tid = transactionEntityFacade.createTransaction(id, -10, "CREDIT");
        Assert.assertTrue(tid != -1);
        // Bananer 0 from account (should fail)
        tid = transactionEntityFacade.createTransaction(id, 0, "BANANER");
        Assert.assertTrue(tid == -1);
    }
    
    /*@Test
    public void testGet() {
        long id = accountEntityFacade.createAccount("testCreatePersonKey", "testCreateBankKey", "CHECK");
        long tid = transactionEntityFacade.createTransaction(id, 0, "DEBIT");
        
        Transaction t = transactionEntityFacade.getTransaction(tid);
        Assert.assertNotNull(t);
        Assert.assertTrue(t.getType().equals("DEBIT"));
        Assert.assertTrue(t.getAccountID() == id);
        Assert.assertTrue(t.getAmount() == 0);
    }*/
    
    @Test
    public void testFindAllTransactionsFor() {
        long id = accountEntityFacade.createAccount("testCreatePersonKey", "testCreateBankKey", "CHECK");
        transactionEntityFacade.createTransaction(id, 0, "DEBIT");
        transactionEntityFacade.createTransaction(id, 0, "CREDIT");
        transactionEntityFacade.createTransaction(id, 0, "DEBIT");
        // Below should not go through
        transactionEntityFacade.createTransaction(id, 0, "BANAN");
        
        List<Transaction> transactions = transactionEntityFacade.findTransactionsFor(id);
        Assert.assertNotNull(transactions);
        Assert.assertEquals(3, transactions.size());
        
        for (Transaction trans : transactions) {
            Assert.assertEquals(trans.getAccount().getID(), id);
        }
    }
}