package se.liu.ida.tdp024.account.logic.impl.facade;

import java.util.List;
import se.liu.ida.tdp024.account.data.api.entity.Transaction;
import se.liu.ida.tdp024.account.data.api.facade.TransactionEntityFacade;
import se.liu.ida.tdp024.account.logic.api.facade.TransactionLogicFacade;

/**
 *
 * @author coolmarle943
 */
public class TransactionLogicFacadeImpl implements TransactionLogicFacade {
    final private TransactionEntityFacade transactionEntityFacade;
    
    public TransactionLogicFacadeImpl(TransactionEntityFacade transactionEntityFacade)
    {
        this.transactionEntityFacade = transactionEntityFacade;
    }
    
    @Override
    public long createTransaction(long accountID, long amount, String type)
    {
        return transactionEntityFacade.createTransaction(accountID, amount, type);
    }
    
    @Override
    public List<Transaction> findTransactionsFor(long id)
    {
        return transactionEntityFacade.findTransactionsFor(id);
    }
}
