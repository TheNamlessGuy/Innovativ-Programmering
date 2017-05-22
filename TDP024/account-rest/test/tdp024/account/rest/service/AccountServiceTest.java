package tdp024.account.rest.service;

import org.junit.After;
import org.junit.Assert;
import org.junit.Test;
import se.liu.ida.tdp024.account.data.api.util.StorageFacade;
import se.liu.ida.tdp024.account.data.impl.db.util.StorageFacadeDB;
import se.liu.ida.tdp024.account.rest.service.AccountService;

public class AccountServiceTest {

    //-- Units under test ---//
    private final StorageFacade storageFacade = new StorageFacadeDB();
    private final AccountService accountService = new AccountService();
    
    private static final String URL = "http://localhost:8080/account-rest/account/";

    @After
    public void tearDown() {
        storageFacade.emptyStorage();

    }

    @Test
    public void testCreate() {
        // Create normally
        String result = accountService.create("CHECK", "coolmarkus", "tjuvbank");
        Assert.assertTrue(result.equals("OK"));
        
        // No accounttype
        result = accountService.create("", "coolmarkus", "tjuvbank");//http.get(URL + "create", "name", "coolmarkus", "bank", "tjuvbank", "accounttype", "");
        Assert.assertTrue(result.equals("FAILED"));
        
        // No name
        result = accountService.create("CHECK", "", "tjuvbank");//http.get(URL + "create", "name", "", "bank", "tjuvbank", "accounttype", "CHECK");
        Assert.assertTrue(result.equals("FAILED"));
        
        // No bank
        result = accountService.create("CHECK", "coolmarkus", "");//http.get(URL + "create", "name", "coolmarkus", "bank", "", "accounttype", "CHECK");
        Assert.assertTrue(result.equals("FAILED"));
        
        // Missing accounttype
        result = accountService.create(null, "coolmarkus", "tjuvbank");//http.get(URL + "create", "name", "coolmarkus", "bank", "tjuvbank");
        Assert.assertTrue(result.equals("FAILED"));
        
        // Missing name
        result = accountService.create("CHECK", null, "tjuvbank");//http.get(URL + "create", "bank", "tjuvbank", "accounttype", "CHECK");
        Assert.assertTrue(result.equals("FAILED"));
        
        // Missing bank
        result = accountService.create("CHECK", "coolmarkus", null);//http.get(URL + "create", "name", "coolmarkus", "accounttype", "CHECK");
        Assert.assertTrue(result.equals("FAILED"));
    }
    
    @Test
    public void testFindName() {
        String result = accountService.findName("Q");//http.get(URL + "find/name/", "name", "Q");
        Assert.assertTrue(result.equals("[]"));
        
        accountService.create("CHECK", "Q", "tjuvbank");
        //http.get(URL + "create", "name", "Q", "bank", "tjuvbank", "accounttype", "CHECK");
        
        result = accountService.findName("Q");//http.get(URL + "find/name/", "name", "Q");
        Assert.assertFalse(result.equals("[]"));
        
        // Don't send a name
        result = accountService.findName(null);//http.get(URL + "find/name/");
        Assert.assertTrue(result.equals("[]"));
        
        // Send nonexistant name
        result = accountService.findName("BANANMANNEN");//http.get(URL + "find/name/", "name", "BANANMANNEN");
        Assert.assertTrue(result.equals("[]"));
    }
    
    @Test
    public void testDebit() {
        String result = accountService.debit(500000, 0);//http.get(URL + "debit", "id", "5000000", "amount", "0");
        Assert.assertTrue(result.equals("FAILED"));
        
        accountService.create("CHECK", "Xena", "tjuvbank");//http.get(URL + "create", "name", "Xena", "bank", "tjuvbank", "accounttype", "CHECK");
        long ID = accountService.id("Xena");//http.get(URL + "id", "name", "Xena");
        
        result = accountService.debit(ID, 0);//http.get(URL + "debit", "id", ID, "amount", "0");
        Assert.assertTrue(result.equals("OK"));
        
        result = accountService.debit(ID, 100);//http.get(URL + "debit", "id", ID, "amount", "100");
        Assert.assertTrue(result.equals("OK")); // Status of transaction is FAILED, though
        
        result = accountService.debit(-1, 0);//http.get(URL + "debit", "amount", "0");
        Assert.assertTrue(result.equals("FAILED"));
        
        result = accountService.debit(ID, -1);//http.get(URL + "debit", "id", ID);
        Assert.assertTrue(result.equals("OK")); // Status of transaction is FAILED, though
        
        result = accountService.debit(-1, -1);//http.get(URL + "debit");
        Assert.assertTrue(result.equals("FAILED"));
    }
    
    @Test
    public void testCredit() {
        String result = accountService.credit(500000, 0);
        Assert.assertTrue(result.equals("FAILED"));
        
        accountService.create("CHECK", "Xena", "tjuvbank");
        long ID = accountService.id("Xena");
        
        result = accountService.credit(ID, 0);
        Assert.assertTrue(result.equals("OK"));
        
        result = accountService.credit(ID, 100);
        Assert.assertTrue(result.equals("OK")); // Status of transaction is FAILED, though
        
        result = accountService.credit(-1, 0);
        Assert.assertTrue(result.equals("FAILED"));
        
        result = accountService.credit(ID, -1);
        Assert.assertTrue(result.equals("OK")); // Status of transaction is FAILED, though
        
        result = accountService.credit(-1, -1);
        Assert.assertTrue(result.equals("FAILED"));
    }
    
    @Test
    public void testTransactions() {
        String result = accountService.transactions(500000);
        Assert.assertTrue(result.equals("[]"));
        
        accountService.create("CHECK", "Xena", "tjuvbank");
        long ID = accountService.id("Xena");
        
        result = accountService.transactions(ID);
        Assert.assertTrue(result.equals("[]"));
        
        result = accountService.credit(ID, 100);//http.get(URL + "credit", "id", ID, "amount", "100");
        Assert.assertTrue(result.equals("OK"));
        
        result = accountService.transactions(ID);//http.get(URL + "transactions", "id", ID);
        Assert.assertFalse(result.equals("[]"));
        
        result = accountService.transactions(-1);//http.get(URL + "transactions");
        Assert.assertTrue(result.equals("[]"));
    }
}