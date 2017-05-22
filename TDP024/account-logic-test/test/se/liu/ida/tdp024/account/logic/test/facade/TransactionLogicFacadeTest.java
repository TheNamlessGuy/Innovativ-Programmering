/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package se.liu.ida.tdp024.account.logic.test.facade;

import java.util.List;
import junit.framework.Assert;
import org.junit.After;
import org.junit.Test;
import se.liu.ida.tdp024.account.data.api.entity.Transaction;
import se.liu.ida.tdp024.account.data.api.util.StorageFacade;
import se.liu.ida.tdp024.account.data.impl.db.facade.AccountEntityFacadeDB;
import se.liu.ida.tdp024.account.data.impl.db.facade.TransactionEntityFacadeDB;
import se.liu.ida.tdp024.account.data.impl.db.util.StorageFacadeDB;
import se.liu.ida.tdp024.account.logic.api.facade.AccountLogicFacade;
import se.liu.ida.tdp024.account.logic.api.facade.TransactionLogicFacade;
import se.liu.ida.tdp024.account.logic.impl.facade.AccountLogicFacadeImpl;
import se.liu.ida.tdp024.account.logic.impl.facade.TransactionLogicFacadeImpl;

/**
 *
 * @author coolmarle943
 */
public class TransactionLogicFacadeTest {

    
    public AccountLogicFacade accountLogicFacade = new AccountLogicFacadeImpl(new AccountEntityFacadeDB());
    //--- Unit under test ---//
    public TransactionLogicFacade transactionLogicFacade = new TransactionLogicFacadeImpl(new TransactionEntityFacadeDB());
    
    public StorageFacade storageFacade = new StorageFacadeDB();
    
    @After
    public void tearDown() {
        storageFacade.emptyStorage();
    }
    
    @Test
    public void testCreate() {
        long accID = accountLogicFacade.createAccount("coolmarkus", "tjuvbank", "SAVINGS");
        
        // Add 100 to account
        long result = transactionLogicFacade.createTransaction(accID, 100, "CREDIT");
        Assert.assertTrue(result != -1);
        // Remove 50 from account
        result = transactionLogicFacade.createTransaction(accID, 50, "DEBIT");
        Assert.assertTrue(result != -1);
        // Remove more than availabe (goes through but is set to FAILED)
        result = transactionLogicFacade.createTransaction(accID, 51, "DEBIT");
        Assert.assertTrue(result != -1);
        // Negative amount (goes through but is set to FAILED)
        result = transactionLogicFacade.createTransaction(accID, -10, "DEBIT");
        Assert.assertTrue(result != -1);
        // Unknown account
        result = transactionLogicFacade.createTransaction(5000000, 100, "CREDIT");
        Assert.assertTrue(result == -1);
        // Unknown type
        result = transactionLogicFacade.createTransaction(accID, 100, "BANANER");
        Assert.assertTrue(result == -1);
        // Null type
        result = transactionLogicFacade.createTransaction(accID, 100, null);
        Assert.assertTrue(result == -1);
    }
    
    @Test
    public void testGetAll() {
        long accID = accountLogicFacade.createAccount("coolmarkus", "tjuvbank", "CHECK");
        transactionLogicFacade.createTransaction(accID, 100, "CREDIT");
        transactionLogicFacade.createTransaction(accID, 100, "DEBIT");
        
        accID = accountLogicFacade.createAccount("coolmarkus", "tjuvbank", "SAVINGS");
        transactionLogicFacade.createTransaction(accID, 100, "CREDIT");
        transactionLogicFacade.createTransaction(accID, 100, "CREDIT");
        transactionLogicFacade.createTransaction(accID, 100, "DEBIT");
        transactionLogicFacade.createTransaction(accID, 50, "DEBIT");
        transactionLogicFacade.createTransaction(accID, 50, "DEBIT");
        transactionLogicFacade.createTransaction(accID, 1, "DEBIT"); // Fails, is still saved        
        
        List<Transaction> trans = transactionLogicFacade.findTransactionsFor(accID);
        Assert.assertEquals(6, trans.size());
        
        trans = transactionLogicFacade.findTransactionsFor(500000);
        Assert.assertEquals(0, trans.size());
    }
}
