package se.liu.ida.tdp024.account.rest.service;

import java.util.List;
import javax.ws.rs.GET;
import javax.ws.rs.Produces;
import javax.ws.rs.Path;
import javax.ws.rs.QueryParam;
import org.codehaus.jettison.json.JSONArray;
import se.liu.ida.tdp024.account.data.api.entity.Account;
import se.liu.ida.tdp024.account.data.api.entity.Transaction;
import se.liu.ida.tdp024.account.data.impl.db.facade.AccountEntityFacadeDB;
import se.liu.ida.tdp024.account.data.impl.db.facade.TransactionEntityFacadeDB;
import se.liu.ida.tdp024.account.logic.api.facade.AccountLogicFacade;
import se.liu.ida.tdp024.account.logic.api.facade.TransactionLogicFacade;
import se.liu.ida.tdp024.account.logic.impl.facade.AccountLogicFacadeImpl;
import se.liu.ida.tdp024.account.logic.impl.facade.TransactionLogicFacadeImpl;
import se.liu.ida.tdp024.account.util.json.AccountJsonSerializer;
import se.liu.ida.tdp024.account.util.json.AccountJsonSerializerImpl;

@Path("/account")
public class AccountService {
    
    /* ACCOUNTS */
    private final AccountLogicFacade accountLogicFacade = new AccountLogicFacadeImpl(new AccountEntityFacadeDB());
    private final AccountJsonSerializer accountJSONSerializer = new AccountJsonSerializerImpl();
    
    @Path("/create")
    @GET
    @Produces("text/plain")
    public String create(@QueryParam("accounttype") String accountType,
                         @QueryParam("name") String personName,
                         @QueryParam("bank") String bankName) {
        long retval = accountLogicFacade.createAccount(personName, bankName, accountType);
        
        if (retval == -1)
            return "FAILED";
        return "OK";
    } 
    
    @Path("/find/name")
    @GET
    @Produces("application/json")
    public String findName(@QueryParam("name") String personName) {
        
        List<Account> list = accountLogicFacade.findAccountsFor(personName);
        
        return accountJSONSerializer.toJson(list);
    }
    
    /* TRANSACTIONS */
    private final TransactionLogicFacade transactionLogicFacade = new TransactionLogicFacadeImpl(new TransactionEntityFacadeDB());
    
    @Path("/debit")
    @GET
    @Produces("text/plain")
    public String debit(@QueryParam("id") long id,
                        @QueryParam("amount") long amount) {
        long retval = transactionLogicFacade.createTransaction(id, amount, "DEBIT");
        
        if (retval == -1)
            return "FAILED";                    
        return "OK";
    }
    
    @Path("/credit")
    @GET
    @Produces("text/plain")
    public String credit(@QueryParam("id") long id,
                         @QueryParam("amount") long amount) {
        long retval = transactionLogicFacade.createTransaction(id, amount, "CREDIT");
        
        if (retval == -1)
            return "FAILED";                    
        return "OK";
    }
    
    @Path("/transactions")
    @GET
    @Produces("application/json")
    public String transactions(@QueryParam("id") long id) {
        List<Transaction> list = transactionLogicFacade.findTransactionsFor(id);
        
        return accountJSONSerializer.toJson(list);
    }
    
    public long id(@QueryParam("name") String name) {
        List<Account> accs = accountLogicFacade.findAccountsFor(name);
        return accs.get(accs.size() - 1).getID();
    }
}