# -*- coding: utf-8 -*-
require './rdparse.rb'
require './niLANGklasser.rb'

##############################################################################
#
# This part defines the niLANG
#
##############################################################################

class NiLANG
        
  def initialize
    @niLANGParser = Parser.new("niLANG") do
      token(/¤[\s\S]*?¤/)
      token(/definiera klass/) {|m| m} #specialfall
      token(/definiera initiering/) {|m| m} #specialfall
      token(/annars om/) {|m| m} #specialfall
      token(/\".*?\"/) {|m| m}
      token(/\s+/)
      token(/(==|\!=|<=|>=|\|\||\&\&)/) {|m| m}
      token(/\d+\.\d+/) {|m| m.to_f } # Floats
      token(/\d+/) {|m| m.to_i } # Integers

      token(/[\w_ÅÄÖåäö]+/) {|m| m }
      token(/./) {|m| m }
      
      start :program do 
        match(:flera_satser) {|satser| Program_nod.new(satser)}

      end
      
      rule :flera_satser do 
        match(:flera_satser, :sats) {|satser, sats| 
          satser += sats
          satser}
        match(:sats) {|sats| sats}
      end

      rule :sats do
        match(:behållare_uttryck, ";") {|uttryck,_| [uttryck]}
        match(:konsol_uttryck, ";") {|uttryck,_| [uttryck]}
        match(:tilldelnings_uttryck, ";") {|uttryck, _| [uttryck] }
        match(:om_block) {|block| [block] }
        match(:förvarje_sats){|sats| [sats] }
        match(:medans_sats) {|sats| [sats]}
        match(:funktions_definition) {|funktion| [funktion]}
        match(:klass_definition){|a|[a]}
        match(:klass_instans, ";") {|a,_|[a]}
        match(:jämförelse_uttryck, ";") {|a,_| a}
        match()
      end
      
      rule :behållare_uttryck do
        match("beh", :sett_variabel, "=", "[", :argument_lista, "]") {|_,var,_,_,arg,_|
          Skapa_behallare_nod.new(var,arg)
        }
        match("beh", :sett_variabel){|_,var|
          Skapa_behallare_nod.new(var)
        }

        match(:sett_variabel,".", "lägg_till", "(", :argument_lista, ")"){|var,_,_,_,arg,_|
          Lagg_till_behallare_nod.new(var,arg)
        }
        match(:sett_variabel,".", "lägg_till", "(", Integer , "blir" ,:argument_lista, ")"){
          |var,_,_,_, index , _,arg,_|
          Lagg_till_behallare_nod.new(var,arg, index)
        }
        match(:sett_variabel,".", "ta_bort", "(", Integer, "till", Integer, ")"){
          |var,_,_,_,index1,_,index2,_|
          Ta_bort_nod.new(var,index1,index2)
        }
        match(:sett_variabel,".", "ta_bort", "(", Integer, ")"){
          |var,_,_,_,index,_|
          Ta_bort_nod.new(var,index)
        }
        match(:sett_variabel,".", "ta_bort", "(", ")"){
          |var,_,_,_,_|
          Ta_bort_nod.new(var)
        }

        match(:sett_variabel,".", "ändra", "(", Integer, "blir" , :aritmetiskt_uttryck, ")"){
          |var,_,_,_,index,_,v,_|
          Andra_behallare_nod.new(var,index, v)
        }

      end

      rule :behållare_egenskap_uttryck do
        match(:sett_variabel,".", "hämta", "(", Integer, ")") {|var,_,_,_,index,_|
          Behallare_egenskap_nod.new(var,index)
        }
        match(:sett_variabel,".", "storlek", "(", ")"){|var,_,_,_,_|
          Behallare_egenskap_nod.new(var)
        }        
      end
    
      rule :tilldelnings_uttryck do
        match(:sett_variabel, "=" , :aritmetiskt_uttryck) {|variabel,op,uttryck| 
          Tilldelnings_nod.new(variabel,op,uttryck) 
        }
        match(:sett_variabel,/(\+|\-|\*)/, "=" , :aritmetiskt_uttryck) {|variabel,op1,op2,uttryck| 
          Tilldelnings_nod.new(variabel, (op1 + op2), uttryck) 
        }
        match(:namn, :sett_variabel ,"=", :aritmetiskt_uttryck) {|typ,variabel,_,v|
          Var_dekl_nod.new(typ,variabel ,v) 
        }
        match(:namn, :sett_variabel) {|typ,variabel|
          Var_dekl_nod.new(typ,variabel) 
          }
      end

      rule :om_block do
        match(:om_sats){|sats| Om_block.new(sats)}
      end
      rule :om_sats do
        match(:om,:annars_om, :annars,"slut"){
          |om_sats,annars_om_sats,annars_sats,_|
          [om_sats] + annars_om_sats + [annars_sats]
        }
        match(:om,:annars_om, "slut"){
          |om_sats,annars_om_sats,_| [om_sats] + annars_om_sats
        }
        match(:om,:annars, "slut"){
          |om_sats,annars_sats,_| [om_sats] + [annars_sats]
        }
        match(:om, "slut") {|om_sats,_| [om_sats]}
      end

      rule :om do
        match("om","(", :jämförelse_uttryck, ")", :flera_satser) { 
          |_,_,jemforelse,_,satser| Om_nod.new(jemforelse,satser)
        }
      end

      rule :annars_om do
        match("annars om", "(", :jämförelse_uttryck, ")", :flera_satser){ 
          |_,_,jemforelse,_,satser| [Om_nod.new(jemforelse,satser)]
        }
        match(:annars_om, :annars_om) {
          |annars_om1,annars_om2| annars_om1 += annars_om2
        }
      end

      rule :annars do 
        match("annars", :flera_satser) {|_,satser| 
          Annars_nod.new(satser)
        }
      end

      rule :förvarje_sats do
        match("för", "varje", "(", :sett_variabel, "till", :förvarje_typ, :sett_variabel, ")", :flera_satser, "slut"){
          |_,_,_,var,_,typ,nyvar,_,satser,_|
          Forvarje_sats_nod.new(var,nyvar, satser, typ)
        }
        match("för", "varje", "(", :sett_variabel, "till", :sett_variabel , ")", :flera_satser, "slut"){
          |_,_,_,var,_,nyvar,_,satser,_|
          Forvarje_sats_nod.new(var,nyvar,satser)
        }
        
      end

      rule :förvarje_typ do
        match("bok")
        match("str")
        match("het")
        match("flt")
      end

      rule :medans_sats do
        match("medans", "(", :jämförelse_uttryck, ")", :flera_satser, "slut") {|_,_,jemforelse,_,satser,_|
          Medans_nod.new(jemforelse, satser)
        }
      end

      rule :konsol_uttryck do
        match("kskriv", :aritmetiskt_uttryck) {|typ,uttryck| 
          Konsol_nod.new(typ, uttryck)
        }
        match("kläs", :sett_variabel) {|typ,uttryck| 
          Konsol_nod.new(typ, uttryck)
        }
      end

      rule :jämförelse_uttryck do
        match(:eller_uttryck) 
      end

      rule :eller_uttryck do
        match(:eller_uttryck, /(eller|\|\|)/, :och_uttryck){|uttryck1,_,uttryck2|
          Eller_nod.new(uttryck1, uttryck2)
        }
        match(:och_uttryck)
      end

      rule :och_uttryck do
        match(:aritmetiskt_uttryck, :bool_operator, :aritmetiskt_uttryck){|uttryck1, op, uttryck2|
          Jemforelse_nod.new(uttryck1, op, uttryck2)
        }
        match(:och_uttryck, /(och|\&\&)/, :jämförelse_uttryck){|uttryck1,_,uttryck2|
          Och_nod.new(uttryck1, uttryck2)
        }
        match(:boolean)
        match(:aritmetiskt_uttryck)
      end

      rule :funktions_definition do
        match("definiera", "funktion", :namn, "(", :parameter_lista, ")", :funktions_satser, "slut") {|_,_,a,_,b,_,c,_| 
          Funktions_nod.new(a,c,b)}
        match("definiera", "funktion", :namn, "(", ")", :funktions_satser, "slut") {|_,_,a,_,_,c,_| 
          Funktions_nod.new(a,c)}
      end

      rule :funktions_satser do
        match(:funktions_satser, :funktions_sats) {|satser,sats| satser + [sats]}
        match(:funktions_sats) {|sats| [sats]}
      end

      rule :funktions_sats do
        match(:behållare_uttryck, ";") {|uttryck,_| [uttryck]}
        match(:retur_uttryck, ";") {|a,_| Funktions_sats_nod.new(a)}
        match(:konsol_uttryck, ";") {|a,_| Funktions_sats_nod.new(a)}
        match(:tilldelnings_uttryck, ";") {|a,_| Funktions_sats_nod.new(a)}
        match(:om_block) {|a| Funktions_sats_nod.new(a)}
        match(:medans_sats) {|a| Funktions_sats_nod.new(a)}
        match(:jämförelse_uttryck, ";") {|a,_| Funktions_sats_nod.new(a)}
        match(:klass_instans, ";") {|a,_| Funktions_sats_nod.new(a)}
        match()
      end
      
      rule :funktions_anrop do
        match(:namn, "(", :argument_lista, ")" ) {|a,_,b,_| 
          Funktions_anrops_nod.new(a,b)
        }
        match(:namn, "(", ")" ) {|a,_,_| 
          Funktions_anrops_nod.new(a)
        }

      end

      rule :mel_funktions_definition do
        match("definiera", "funktion", :namn, "(", :parameter_lista, ")", :mel_funktions_satser, "slut") {|_,_,a,_,b,_,c,_| 
          Mel_funktions_nod.new(a,c,b)}
        match("definiera", "funktion", :namn, "(", ")", :mel_funktions_satser, "slut") {|_,_,a,_,_,c,_| 
          Mel_funktions_nod.new(a,c)}
      end

     rule :mel_funktions_satser do
        match(:mel_funktions_satser, :mel_funktions_sats) {|satser,sats| satser + [sats]}
        match(:mel_funktions_sats) {|sats| [sats]}
      end

      rule :mel_funktions_sats do
        match(:behållare_uttryck, ";") {|uttryck,_| [uttryck]}
        match(:retur_uttryck, ";") {|a,_| Funktions_sats_nod.new(a)}
        match(:konsol_uttryck, ";") {|a,_| Funktions_sats_nod.new(a)}
        match(:tilldelnings_uttryck, ";") {|a,_| Funktions_sats_nod.new(a)}
        match(:om_block) {|a| Funktions_sats_nod.new(a)}
        match(:medans_sats) {|a| Funktions_sats_nod.new(a)}
        match(:jämförelse_uttryck, ";") {|a,_| Funktions_sats_nod.new(a)}
        match()
      end

      rule :mel_funktions_anrop do
        match(:sett_variabel, ".", :mel_funktion) {|a,_,b| 
          Mel_funktions_anrops_nod.new(a,b)
        }
      end
      
      
      rule :mel_funktion do
        match(:namn, "(", :argument_lista, ")" ) {|a,_,b,_| 
          [a,b]
         # Djupt_anrop_nod.new(a,b)
        }
        match(:namn, "(", ")" ) {|a,_,_| 
          [a,nil]
         # Djupt_anrop_nod.new(a)
        }
      end


      rule :klass_definition do
        match("definiera klass", :namn, :init_sats ,:klass_satser, "slut"){
          |_,namn,init,satser,_|
          Klass_nod.new(namn,init,satser)
        }
        match("definiera klass", :namn, :klass_satser, "slut"){
          |_,namn,init,satser,_|
          init = nil
          Klass_nod.new(namn,init,satser)
        }
      end

      rule :klass_satser do
        match(:klass_satser,:klass_sats) {|satser,sats| satser + [sats]}
        match(:klass_sats) {|sats| [sats]}
      end

      rule :klass_sats do
        match(:mel_funktions_definition) {|funktion| 
          Klass_sats_nod.new(funktion)
        }
        match()
      end
      
      rule :init_sats do
        match("definiera initiering", "(", :parameter_lista, ")" , :mel_funktions_satser, "slut"){
          |_,_,a,_,b,_| [a,b]
        }
        match("definiera initiering", "(", :parameter_lista, ")" , "slut"){
          |_,_,a,_,b,_| [a,[]]
        }
        match("definiera initiering", "(", ")" , :klass_satser, "slut"){
          |_,_,_,b,_| [b]
        }
      end

      rule :klass_instans do
        match(:namn, :sett_variabel, "(", :argument_lista, ")") {|a,b,_,c,_|  
          Klass_instans_nod.new(a,b,c)}
        match(:namn, :sett_variabel) {|a,b|  
          Klass_instans_nod.new(a,b)
        }
      end
      
      rule :aritmetiskt_uttryck do
        match(:aritmetiskt_uttryck, /[\+\-]/, :multi_uttryck) {|vh,op,hh| 
          Aritmetisk_nod.new(vh,op,hh)
        }
        match(:multi_uttryck){|uttryck| uttryck }
      end

      rule :multi_uttryck do
        match(:multi_uttryck, /[\*\/]/, :faktor_uttryck) {|vh,op,hh| 
          Aritmetisk_nod.new(vh,op,hh) 
        }
        match(:faktor_uttryck){|uttryck| uttryck }
      end

      rule :faktor_uttryck do
        match(:prio_uttryck, "^", :faktor_uttryck) {|vh,op,hh| 
          Aritmetisk_nod.new(vh,op,hh) 
        }
        match(:prio_uttryck){|uttryck| uttryck }
      end

      rule :prio_uttryck do
        match("(", :aritmetiskt_uttryck, ")") {|_,uttryck,_| uttryck }
        match(:fragment) {|uttryck| uttryck }
      end

      rule :fragment do
        match("[", :argument_lista, "]") {|_,v,_|Fragment_behallare_nod.new(v)}
        match(:funktions_anrop) {|funktions_anrop| [funktions_anrop]}
        match(:behållare_egenskap_uttryck) {|v| Fragment_nod.new(v)}
        match(:mel_funktions_anrop){|funktions_anrop| [funktions_anrop]}
        match(:boolean){|v| Fragment_nod.new(v)}
        match(/\".*?\"/) {|v| Fragment_nod.new(v[1..-2])}
        match(:variabel) {|v| v}
        match("-", Float){|_,v| Fragment_nod.new(v*-1)}
        match(Float){|v| Fragment_nod.new(v)}
        match("-", Integer){|_,v| Fragment_nod.new(v*-1)}
        match(Integer){|v| Fragment_nod.new(v)}


      end

      rule :parameter_lista do
        match(:parameter_lista, ",", :namn, :sett_variabel){|param_lista,_,namn,variabel| 
          param_lista + [[namn.eval,variabel.eval]]
        }
        match(:namn, :sett_variabel){|namn,variabel|
          [[namn.eval,variabel.eval]]}
      end
      
      rule :argument_lista do
        match(:argument_lista, ",", :aritmetiskt_uttryck){|arg_lista,_,fragment|
          arg_lista + [fragment]
        }
        match(:aritmetiskt_uttryck) {|fragment|
          [fragment]
        }
      end

      rule :retur_uttryck do
        match("returnera", :aritmetiskt_uttryck) {|_,fragment|
          Retur_nod.new(fragment)
        }
      end
      rule :namn do
        match(:datatyp)
        match(/[^(slut)][a-zA-ZåäöÅÄÖ]*/)
      end

      rule :datatyp do
        match("het"){|v| Datatyp_nod.new(v)}
        match("flt"){|v| Datatyp_nod.new(v)}
        match("str"){|v| Datatyp_nod.new(v)}
        match("boo"){|v| Datatyp_nod.new(v)}
      end

      rule :bool_operator do
        match("==")
        match("!=")
        match("<")
        match("<=")
        match(">")
        match(">=")
      end

      rule :boolean do
        match("sant")  {|v|Boolean_nod.new(v)}
        match("falskt") {|v|Boolean_nod.new(v)} 
      end

      rule :variabel do
        match(/[A-Za-zåäöÅÄÖ]+/) {|a| Variabel_nod.new(a) }
      end

      rule :sett_variabel do
        match(/[A-Za-zåäöÅÄÖ]+/) {|a| Sett_variabel_nod.new(a) }
      end

    end
  end
  
  def done(str)
    ["quit","exit","bye",""].include?(str.chomp)
  end
  
  def niLANG(filnamn)
   # begin
      @niLANGParser.logger.level = Logger::WARN

      puts "niLang börjar\n"
      #    str = gets
      str = File.open(filnamn).read
      #  puts str
      if done(str) then
        puts "Bye."
      else
        #puts "=> #{@niLANGParser.parse str}"
        result = @niLANGParser.parse(str)
        result.eval
      end
    #rescue
     # puts "Segment fel!"
    #end
  end

  def log(state = true)
    if state
      @niLANGParser.logger.level = Logger::DEBUG
    else
      @niLANGParser.logger.level = Logger::WARN
    end
  end
end

NiLANG.new.niLANG(ARGV[0])
