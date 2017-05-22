package se.liu.ida.tdp024.account.xfinal.test.account;

import java.util.ArrayList;
import java.util.List;
import org.junit.Assert;
import org.junit.Test;
import se.liu.ida.tdp024.account.util.http.HTTPHelper;
import se.liu.ida.tdp024.account.util.http.HTTPHelperImpl;
import se.liu.ida.tdp024.account.xfinal.test.util.FinalConstants;

public class AccountCreateTest {

    private static final HTTPHelper httpHelper = new HTTPHelperImpl();

    @Test
    public void createSuccess() {

        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("OK", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("OK", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "CHECK";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("OK", response);
        }
        {
            String name = "Jakob Pogulis";
            String bank = "NORDEA";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("OK", response);
        }
        {
            String name = "Jakob Pogulis";
            String bank = "NORDEA";
            String accountType = "CHECK";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("OK", response);
        }
        {
            String name = "Jakob Pogulis";
            String bank = "SWEDBANK";
            String accountType = "CHECK";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("OK", response);
        }
        {
            String name = "Zorro";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("OK", response);
        }
        {
            String name = "Zorro";
            String bank = "JPMORGAN";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("OK", response);
        }
        {
            String name = "Zorro";
            String bank = "NORDNET";
            String accountType = "CHECK";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("OK", response);
        }

    }

    @Test
    public void createSuccessAllCombos() {

        List<String> personNames = new ArrayList<String>();
        List<String> bankNames = new ArrayList<String>();
        List<String> accountTypes = new ArrayList<String>();

        personNames.add("Jakob Pogulis");
        personNames.add("Xena");
        personNames.add("Marcus Bendtsen");
        personNames.add("Zorro");
        personNames.add("Q");

        bankNames.add("SWEDBANK");
        bankNames.add("IKANOBANKEN");
        bankNames.add("JPMORGAN");
        bankNames.add("NORDEA");
        bankNames.add("CITIBANK");
        bankNames.add("HANDELSBANKEN");
        bankNames.add("SBAB");
        bankNames.add("HSBC");
        bankNames.add("NORDNET");

        accountTypes.add("CHECK");
        accountTypes.add("SAVINGS");

        for (String personName : personNames) {
            for (String bankName : bankNames) {
                for (String accountType : accountTypes) {
                    String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", personName, "bank", bankName, "accounttype", accountType);
                    Assert.assertEquals("OK", response);
                }
            }
        }

    }

    @Test
    public void createFailure() {

        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "CREDITCARD";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "Marcus";
            String bank = "SWEDBANK";
            String accountType = "CHECK";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "LEHMAN";
            String accountType = "CHECK";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "";
            String accountType = "CHECK";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "";
            String bank = "SWEDBANK";
            String accountType = "CHECK";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "";
            String bank = "SWEDBANK";
            String accountType = "CHECK";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "";
            String bank = "";
            String accountType = "CHECK";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "";
            String bank = "";
            String accountType = "";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "";
            String bank = "";
            String accountType = "CHECK";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "";
            String bank = "";
            String accountType = "CREDITCARD";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVING"; //It should be SAVINGS
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "bank", bank, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name, "bank", bank);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "name", name);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "bank", bank);
            Assert.assertEquals("FAILED", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/", "accounttype", accountType);
            Assert.assertEquals("FAILED", response);
        }

        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/create/");
            Assert.assertEquals("FAILED", response);
        }


        /* Wrong endpoint (i.e. incorrect request) */
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("", response);
        }
        {
            String name = "Marcus Bendtsen";
            String bank = "SWEDBANK";
            String accountType = "SAVINGS";
            String response = httpHelper.get(FinalConstants.ENDPOINT + "account/account/create/", "name", name, "bank", bank, "accounttype", accountType);
            Assert.assertEquals("", response);
        }



    }
}