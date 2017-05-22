# -*- coding: utf-8 -*-
@@variables = [{}] #Sparar alla variabler
@@scope = 0 #sätter scopet för variabler
@@klass_scope = 0 #sätter scopet för variabler tillhörande klasser

@@scope_stopp = [] #Håller koll på att man inte går för långt bakåt för funktioner

@@funktioner= {} #Sparar alla funktioner
@@funktions_var = {} #sparar parametrar till funktioner.
@@retur_v = nil #Returvärden

#@@klass_definitioner = {}
@@klass_funktioner = {} #Klassfunktioner
@@klass_var = [{}] #Medlemsvariabler

@@temp_klass_var = ["",{}]
#@@temp_klass_param = {}
@@temp_klass_funktioner = {}



def mer_scope
  @@scope += 1
  @@variables.push({})
end

def mindre_scope
  @@scope -= 1
  @@variables.pop()

  if @@scope < 0
    puts "Något gick åt helvete med scopet!"
  end
end

def mer_klass_scope
  @@klass_scope += 1
  @@klass_var.push({})
end

def mindre_klass_scope
  @@klass_scope -= 1
  @@klass_var.pop()

  if @@klass_scope < 0
    puts "Något gick åt helvete med scopet!"
  end
end

#Returnerar värdet i variabeln
def hitta_var(namn)
  i = @@scope

  if(@@temp_klass_var[1].has_key?(namn))
    return @@temp_klass_var[1][namn]
  end

  if(@@scope_stopp == [])
    while(i >= 0)
      if(@@variables[i].has_key?(namn))
        return (@@variables[i][namn])
      end
      i -= 1
    end    
  else
    while(i >= @@scope_stopp.last)
      if(@@variables[i].has_key?(namn))
        return (@@variables[i][namn])
      end
      i -= 1
    end    
  end
end

#Returnerar värdet i klassvariabeln
def hitta_klass_var(namn)
  i = @@scope
    while(i >= 0)
      if(@@klass_var[i].has_key?(namn))
        return (@@klass_var[i][namn])
      end
      i -= 1
    end    
end

#Kollar så att parametrarna datatyp stämmer med argumentens
def kolla_typer(parametrar, argument)
  allt_okej = true

  if(parametrar[0].class != [].class)
    parametrar = [parametrar]
  end

  0.upto((argument.length)-1) do |i|
    case parametrar[i][0]
    when "str"
       allt_okej = (String == argument[i].class)
    when "het"
       allt_okej = (Fixnum == argument[i].class)
    when "ftl"
       allt_okej = (Float == argument[i].class)
    when "boo"
       allt_okej = (TrueClass == argument[i].class || FalseClass == argument[i].class)
    else
      puts("egna klasser")
    end
    if(allt_okej == false)
      return allt_okej
    end
  end

  return allt_okej
end

#Ge en variabel ett värde
def sett_klass_var(klass_var_namn, var_namn)
  i = @@scope

  while ( i >= 0)
    if(@@klass_var[i].has_key?(klass_var_namn))
      v = hitta_var(var_namn)
      if(v != nil)
        @@klass_var[i][klass_var_namn][1][var_namn] = v
      else
        puts "något gick fel med tilldelning"
      end
    end
    i -= 1
  end
end

#Hittar vilket scope en variabel ligger i och om den finns
def variabel_scope(variabel, i)
  hittat = false
  if(@@scope_stopp.length == 0)
      while( i >= 0 )
        if(@@variables[i].has_key?(variabel) )
          hittat = true
          break
        else
          i -= 1
        end
      end
    else
      while( i >= @@scope_stopp.last )
        if(@@variables[i].has_key?(variabel) )
          hittat = true
          break
        else
          i -= 1
        end
      end
    end

  return[hittat, i]
end

#Används när det finns nestlade behållare i en behållare
def djup_behallare_eval(v)
  temp = []
  if(v.class == Fragment_nod)
    return v.eval
  elsif(v.class == Fragment_behallare_nod)
    temp_v = v.eval
    temp_v.each do|var|
      temp.push(djup_behallare_eval(var))
    end
  else
    return(v)
  end
  return temp
end

#Skapar en behållare nod
class Skapa_behallare_nod
  def initialize(variabel, argument = nil)
    @variabel = variabel
    @argument = argument
  end
  
  def eval()
    hittat = false
    variabel = @variabel.eval
    i = @@scope
    
    temp_arg = []

    #Om en behållare har initieringsvärden så måste de
    #evalueras och läggas till
    if(@argument != nil)
      @argument.each do |arg|
        temp_arg.push(arg.eval)
      end
    end

    @@variables[@@scope][variabel] = temp_arg
  
  end
end

#Lägg till värden i en behållare
class Lagg_till_behallare_nod
  def initialize(variabel, argument = nil, index = nil)
    @variabel = variabel
    @argument = argument
    @index = index
  end

  def eval()
    hittat = false
    variabel = @variabel.eval
    i = @@scope
    index = @index
    

    #Om en behållare har initieringsvärden så måste de
    #evalueras och läggas till
    temp_arg = []
    if(@argument != nil)
      @argument.each do |arg|
        temp_arg.push(arg.eval)
      end
    end

    hittat, i = variabel_scope(variabel,i)
    #Om variabeln finns, lägger till antingen ett eller
    #flera värden på ett visst index eller i slutet
    if(hittat == true)
      if(index == nil)
        @@variables[i][variabel] += temp_arg
      else
        0.upto(temp_arg.length-1) do |j|
          @@variables[i][variabel].insert((index+j), temp_arg[j])
        end
      end
    else
      puts "Variabeln fanns inte!"
    end
  end
end

#Ändrar ett värde i en behållare på ett visst index
class Andra_behallare_nod
  def initialize(variabel, index, v)
    @variabel = variabel
    @index = index
    @v = v
  end

  def eval()
    hittat = false
    variabel = @variabel.eval
    i = @@scope
    index = @index
    v = @v.eval

    hittat, i = variabel_scope(variabel,i)

    if(hittat == true)

      #Se till att indexet inte går utanför behållarens gränser 
      # och isåfall ändrar värdet
      if(@@variables[i][variabel].length-1 > index && index >= 0) 
        @@variables[i][variabel][index] = v
      else
        puts "Indexet är utanför behållarens längd!"
      end
    else
      puts "Variabeln fanns inte!"
    end
  end
end

#Hämtar ut längden av eller värdet på ett visst index i en behållare
class Behallare_egenskap_nod
  def initialize(variabel, index = nil)
    @variabel = variabel
    @index = index
  end

  def eval()
    hittat = false
    variabel = @variabel.eval
    i = @@scope
    index = @index

    hittat, i = variabel_scope(variabel,i)

    if(hittat == true)
      
      #Om inte något index skickas med ska vi returnera
      # längden av behållaren
      if(index == nil)
        return @@variables[i][variabel].length

        #Annars Returneras värdet på indexet
      else
        if(@@variables[i][variabel].length-1 > index && index >= 0)
          return @@variables[i][variabel][index]
        else
          puts "Indexet är utanför behållarens längd!"
        end
      end
    else
      puts "Variabeln fanns inte!"
    end
  end
end

#Tar bort ett värde på ett intervall, ett specifikt index
# eller i slutet av en behållare
class Ta_bort_nod
  def initialize(variabel, index1 = nil ,index2 = nil)
    @variabel = variabel
    @index1 = index1
    @index2 = index2
  end

  def eval()
    hittat = false
    variabel = @variabel.eval
    i = @@scope

    intervall = nil
    if(@index2 != nil and @index1 != nil)
      intervall = @index2-@index1;

      #Om ett intervall blir negativt är det fel index helt enkelt
      if (intervall < 0)
        puts "Intervallet får inte bli negativt"
        return nil
      end
    end

    hittat, i = variabel_scope(variabel,i)
    
    #Om vi har ett intervall så ska flera värden tas bort,
    #annars om endast ett index finns så ska den platsen
    #tas bort. Annars så ska sista värdet i behållaren as bort.
    if(hittat == true)
      if(intervall != nil)
        0.upto(intervall) do 
          @@variables[i][variabel].delete_at(@index1)
        end
      elsif(@index1 != nil)
        @@variables[i][variabel].delete_at(@index1)        
      else
        @@variables[i][variabel].delete_at(@@variables[i][variabel].length-1)
      end
    else
      puts "Variabeln fanns inte!"
    end
  end
end

#Huvudnoden som kallar på att evaluera alla undernoder
class Program_nod
  attr_accessor :satser
  def initialize(flera_satser)
    @satser = flera_satser
  end

  def eval()
    @satser.each do | sats |
      sats.eval
    end

    #Debugg-utskrivning för sparade variabler
    if(false)
      puts "Har kommer alla variabler:  "
      puts (@@variables)
      puts "Har kommer alla klass variabler:  "
      puts (@@klass_var)
    end
  end
end

#En klass för att evaluera alla om/annars och annars om satser
class Om_block
  def initialize(satser)
    @satser = satser
  end

  def eval()
    @satser.each do |sats|
      if(sats.eval == true)
        break
      end
    end
    return nil
  end
end

#Evaluera satser i en om nod
class Om_nod
  def initialize(jemforelse, satser)
    @jemforelse = jemforelse
    @satser = satser
  end
  def eval()
    mer_scope()
    mer_klass_scope()

    #om jämförelsen är sann ska satserna i satsen evalueras
    if(@jemforelse.eval == true)
      @satser.each do |sats|
        sats.eval
      end
      mindre_scope()
      mindre_klass_scope()

      return true
    end
    mindre_scope()
    mindre_klass_scope()
    return false
  end
end

#Evaluera satser i en annars nod
class Annars_nod
  def initialize(satser)
    @satser = satser
  end

  def eval()
    mer_scope()
    mer_klass_scope()
    @satser.each do |sats|
      sats.eval
    end
    mindre_scope()
    mindre_klass_scope()
    return true
  end
end

#Används för variabel deklaration
class Var_dekl_nod
  def initialize(typ,variabel,v = nil)
    @typ = typ
    @variabel = variabel
    @v = v
  end

  def eval()
    variabel = @variabel.eval

    #Om v är från ett mel_funktions_kall
    if(@v.class == [].class)
      @v = @v[0]
    end

    #Om v == nil vill vi ge v ett init värde. 
    #Annars ger sparar vi variabeln med en evaluering av v
    if(@v == nil)
      typ = @typ.eval
      case typ
      when "het"
        return @@variables[@@scope][variabel] = 0
      when "flt"
        return @@variables[@@scope][variabel] = 0.0
      when "str"
        return @@variables[@@scope][variabel] = ""
      when "boo"
        return @@variables[@@scope][variabel] = True
      else
        return @@variables[@@scope][variabel] = @v
      end
    end
    return @@variables[@@scope][variabel] = @v.eval
  end
end

#Används för att tilldela existerande variabler värden
class Tilldelnings_nod
  def initialize(variabel, op, uttryck)
    @variabel = variabel
    @op = op
    @uttryck = uttryck
  end

  def eval()
    hittat = false
    variabel = @variabel.eval
    uttryck = @uttryck.eval
    i = @@scope

    hittat, i = variabel_scope(variabel,i)

    #Om variabeln finns ska den uppdateras på valt sätt
    if(hittat == true)
      case @op
      when "="
        @@variables[i][variabel] = uttryck
      when "+="
        @@variables[i][variabel] += uttryck
      when "*="
        @@variables[i][variabel] *= uttryck
      when "-="
        @@variables[i][variabel] -= uttryck
      end
    else
      puts "Variabeln fanns inte"
    end
  end
end

#Används för för_varje satser
class Forvarje_sats_nod
  def initialize(variabel, nyvariabel, satser,typ = nil)
    @variabel = variabel
    @nyvariabel = nyvariabel
    @satser = satser
    @typ = typ
  end

  def eval()
    hittat = false
    variabel = @variabel.eval
    nyvariabel = @nyvariabel.eval
    i = @@scope

    hittat, i = variabel_scope(variabel,i)
    
    #Om vi hittar variabeln vi loopar igenom ska vi loopas igenom på rätt sätt.
    if(hittat == true)
      iter_var = @@variables[i][variabel]
      mer_scope()
      mer_klass_scope()
      if(@typ != nil)
        case @typ
        when "bok"

          #Här ska vi ta ut varje bokstav i variabeln
          0.upto(iter_var.length-1) do |i|
            @@variables[@@scope][nyvariabel] = iter_var[i]
            @satser.each do |sats|
              sats.eval
            end
          end
        when "str"

          #Här ska vi ta ut varje ord i variabeln
          iter_var.split.each do |var|
            @@variables[@@scope][nyvariabel] = var
            @satser.each do |sats|
              sats.eval
            end
          end
        when "het"

          #Här ska vi ta ut varje heltal i variabeln
          iter_var.split.each do |var|
            @@variables[@@scope][nyvariabel] = var.to_i
            @satser.each do |sats|
              sats.eval
            end
          end
        when "flt"

          #Här ska vi ta ut varje flyttal i variabeln
          iter_var.split.each do |var|
            @@variables[@@scope][nyvariabel] = var.to_f
            @satser.each do |sats|
              sats.eval
            end
          end
        else
          puts "Felaktig typ angiven!"
        end
      else

        #Här ska vi ta ut varje element i en behållare.
        iter_var.each do |var|
          @@variables[@@scope][nyvariabel] = var
          @satser.each do |sats|
            sats.eval
          end
        end
      end
      mindre_scope()
      mindre_klass_scope()
    else
      puts "Variabeln hittades inte"
    end

  end
end

#Används för medans loopar.
class Medans_nod
  def initialize(jemforelse, satser)
    @jemforelse = jemforelse
    @satser = satser
  end

  def eval()
    mer_scope()
    mer_klass_scope()

    #Om jämförelsen är sann ska alla satser i loopen evalueras
    while(@jemforelse.eval == true)
      @satser.each do |sats| 
        sats.eval
      end
    end
    mindre_scope()
    mindre_klass_scope()
  end
end

#Används för att skriva ut variabler eller läsa in inmatning.
class Konsol_nod
  def initialize(typ, uttryck)
    @typ = typ
    @uttryck = uttryck
  end

  def eval()

    #Om vi vill Skriva ut en variabel
    if(@typ == "kskriv")

      #Vid uthämtning från en funktion som returnerar ett värde så sparas det i en array.
      #I detta fall vet vi att det bara finns ett element i arrayen.
      if(@uttryck.class == [].class)
        print @uttryck[0].eval
        puts
      else
        print @uttryck.eval
        puts
      end

    #Om vi vill ta en inmatning och variabeln existerar, 
    #vill vi läsa in inmatningen till en korrekt datatyp
    elsif(@typ == "kläs")
      hittat = false
      i = @@scope
      uttryck = @uttryck.eval

      hittat, i = variabel_scope(@uttryck.eval,i)
      if(hittat == true)
        print ("Inmatning: ")
        temp_var = gets
        klass = @@variables[i][uttryck].class
        if (klass == Fixnum)
          @@variables[i][uttryck] = temp_var[0..-2].to_i          
        elsif (klass == Float)
          @@variables[i][uttryck] = temp_var[0..-2].to_f
        elsif (klass == String)
          @@variables[i][uttryck] = temp_var[0..-2]
        else
          puts "Fel typ"
        end
      else
        puts "Variabeln fanns inte"
      end
    else
      puts("Något gick åt helvete med konsol uttryck")
    end
  end
end

#Används för att beräkna aritmetiska uttryck
class Aritmetisk_nod
  def initialize(vh, op, hh)
    @vh = vh
    @op = op
    @hh = hh
  end

  def eval()

    #Om vi använder funktions kall i ett uttryck
    if(@vh.class == [].class)
      vh = @vh[0].eval
    else
      vh = @vh.eval
    end
    if(@hh.class == [].class)
      hh = @hh[0].eval
    else
      hh = @hh.eval
    end

    if(hh == true)
      hh = "sant"
    elsif(hh == false)
      hh = "falskt"
    end
    
    if(vh == true)
      vh = "sant"
    elsif(vh == false)
      vh = "falskt"
    end

    #Olika operatorer ska utföra olika saker
    case @op
    when '+'
      
      #Om vi försöker addera ints och floats
      if((vh.class == Fixnum and hh.class == Float) ||
         (hh.class == Fixnum and vh.class == Float))
        return vh+hh
      end
      
      if(vh.class != hh.class)
        vh = vh.to_s
        hh = hh.to_s
      end
      return vh + hh
    when '-'
      return vh - hh
    when '*'

      #För att hantera omvänd ordning av sträng och int samt 
      #int och array i multiplikation 
      if((vh.class == Fixnum && hh.class == String) || 
         (vh.class == Fixnum && hh.class == Array))
        return hh * vh
      end
      return vh * hh
    when '/'
      return vh / hh
    when '^'
      return vh ** hh
    end
  end
end

#Används för att spara funktioner
class Funktions_nod
  def initialize(namn, satser, param_lista = nil)
    @namn = namn
    @satser = satser
    @param_lista = param_lista
  end
    
  def eval()
    @@funktioner[@namn] = @satser
    @@funktions_var[@namn] = @param_lista
    return @namn
  end
end

#Används för att spara ner satser till en funktion 
#som senare sparas till en klass
class Mel_funktions_nod
  def initialize(namn, satser, param_lista = nil)
    @namn = namn
    @satser = satser
    @param_lista = param_lista
  end
    
  def eval()
    @@temp_klass_funktioner[@namn] = [@param_lista, @satser]
    return @namn
  end
end

#Evaluera alla satser i en funktion när den kallas på
class Funktions_sats_nod
  def initialize(sats)
    @sats = sats
  end
    
  def eval()
    temp_satser = []
    if(@sats.class == [].class)
      @sats.each do |sats|
        temp_satser.push(sats.eval)
      end
      return temp_satser
    end
   
    return @sats.eval
  end
end

#Används för att utföra satserna i en funktion
class Funktions_anrops_nod
  def initialize(namn, argument = nil)
    @namn = namn
    @argument = argument
  end

  def eval()

    #Argumenten måste evalueras då de kommer som noder
    temp_args = [] 
    if( @argument != nil)
      @argument.each do |arg|
        temp_args.push(arg.eval)
      end
    end

    mer_scope()
    mer_klass_scope()
    @@scope_stopp.push(@@scope)

    #Kollar om en funktion existerar
    if(@@funktioner.has_key?(@namn))

      #Kollar om en funktion har parametrar
      if(@@funktions_var[@namn] != nil)

        #Kollar så att parametrarnas och argumentens typer matchar
        #Om dom gör de, spara variablerna i detta scope
        if(kolla_typer(@@funktions_var[@namn], temp_args) == true)
          0.upto((temp_args.length)-1) do |i|
            @@variables[@@scope][@@funktions_var[@namn][i][1]] = temp_args[i]
          end
        else
          puts("Fel på paramterlistan och argumentlistan")
        end

        #Evaluera uttrycken i funktionen
        @@funktioner[@namn].each do |sats|
          sats.eval

          #Ett returvärde har skapats så det ska avbrytas
          if(@@retur_v != nil)
            break;
          end
        end
      else
        @@funktioner[@namn].each do |sats|
          sats.eval

          #Ett returvärde har skapats så det ska avbrytas
          if(@@retur_v != nil)
            break;
          end
        end
      end
    else
      puts("funktionen fanns inte")
    end

    mindre_scope()
    mindre_klass_scope()
    @@scope_stopp.pop()

    #Om ett returvärde finns ska det returneras
    if(@@retur_v != nil)
      temp_v = @@retur_v
      @@retur_v = nil
      return temp_v
    end
    return nil
  end
end

#Används när man kallas på en klassfunktion
class Mel_funktions_anrops_nod
   def initialize(namn, funktions_anrop)
    @namn = namn
    @funktions_anrop = funktions_anrop
   end

   def eval()
     namn = @namn.eval

     var_info = hitta_klass_var(namn)
     klass_namn = var_info[0]
     funktions_namn = @funktions_anrop[0]
     funktions_satser = @@klass_funktioner[var_info[0]][1][funktions_namn][1]
     parametrar =  @@klass_funktioner[klass_namn][1][funktions_namn][0]


     #Argumenten måste evalueras då de kommer som noder
     temp_args = [] 
     if(@funktions_anrop[1] != nil)
       @funktions_anrop[1].each do |arg|
         temp_args.push(arg.eval)
       end
     end

     mer_scope()
     mer_klass_scope()
     @@scope_stopp.push(@@scope)
     
     #Lägger till klass variabler i detta scope
     klassens_variabler = hitta_klass_var(namn)[1]
     if(klassens_variabler != nil)
       klassens_variabler.each do |var|
         @@variables[@@scope][var[0]] = var[1]
       end
     end

     #Kollar så att parametrarnas och argumentens typer matchar
     #Om dom gör de, spara variablerna i detta scope
     if(temp_args.length != 0)
       if(kolla_typer(parametrar, temp_args) == true)
         0.upto((temp_args.length)-1) do |i|
           @@variables[@@scope][parametrar[i][1]] = temp_args[i]
         end
       else
         puts("Fel på paramterlistan och argumentlistan")
       end
     end

     #Evaluera uttrycken i funktionen och
     #avbryter om ett returvärde skapas
     funktions_satser.each do |sats|
       sats.eval
       if(@@retur_v != nil)
         break;
       end
     end
     
     #Uppdaterar klassens variabler
     klassens_variabler.each do |n, v|
       sett_klass_var(namn, n)
     end

     mindre_scope()
     mindre_klass_scope()
     @@scope_stopp.pop()

     #Returnerar returvärde om det finns
     if(@@retur_v != nil)
       temp_v = @@retur_v
       @@retur_v = nil
       return temp_v
     end
     return nil
   end
end

#Används för att skapa en klass
class Klass_nod
  def initialize(namn, init, satser)
    @namn = namn
    @init = init
    @satser = satser
  end

  def eval()
    @satser.each do |sats|
      sats.eval
    end
    @@klass_funktioner[@namn] =  [@init, @@temp_klass_funktioner]
    @@temp_klass_funktioner = {}
    return @namn
  end
end

#Används för att evaluera alla klassens satser
class Klass_sats_nod
  def initialize(sats)
    @sats = sats
  end
    
  def eval()
    temp_satser = []
    if(@sats.class == [].class)
      @sats.each do |sats|
        temp_satser.push(sats.eval)
      end
      return temp_satser
    end

    return @sats.eval
  end
end

#Används för att skapa en klassinstans
class Klass_instans_nod
  def initialize(klassnamn, namn, argument = nil)
    @klassnamn = klassnamn
    @namn = namn
    @argument = argument
  end

  def eval()
    @namn = @namn.eval

    #Argumenten måste evalueras
    temp_args = [] 
    @argument.each do |arg|
      temp_args.push(arg.eval)
    end

    mer_scope()
    @@scope_stopp.push(@@scope)

    #Hämtar initierings delen av klassen
    init = nil

    if( @@klass_funktioner[@klassnamn] != nil)
      init = @@klass_funktioner[@klassnamn][0]
    end


    #Kollar så att parametrarnas och argumentens typer matchar
    #Om dom gör de, spara variablerna i detta scope
    if(kolla_typer(init[0], temp_args) == true)
      0.upto((temp_args.length)-1) do |i|
        @@variables[@@scope][init[0][i][1]] = temp_args[i]
      end
    else
      puts("Fel på paramterlistan och argumentlistan")
    end    
    

    #Evaluerar initialiseringen
    if(init != nil)
      init[1].each do |sats|
        sats.eval
      end
    else
      puts("Fel antal argument")
    end
    
    variabler = [@klassnamn ,@@variables[@@scope]]
    mindre_scope()
    @@scope_stopp.pop()
    
    #Sparar ner klassens variabler
    @@klass_var[@@klass_scope][@namn] = variabler
  end
end

#Används för att jämföra uttryck
class Jemforelse_nod
  def initialize(uttryck1, op, uttryck2)
    @uttryck1 = uttryck1
    @op = op
    @uttryck2 = uttryck2
  end

  def eval()

    #Ifall uttrycket kom ifrån ett anrop
    if(@uttryck1.class == [].class)
      @uttryck1 = @uttryck1[0]
    end

    if(@uttryck2.class == [].class)
      @uttryck2 = @uttryck2[0]
    end

    #Utför vald operator på uttrycken
    case @op
    when "=="
      return @uttryck1.eval == @uttryck2.eval
    when "!="
      return @uttryck1.eval != @uttryck2.eval
    when "<"
      return @uttryck1.eval < @uttryck2.eval
    when "<="
      return @uttryck1.eval <= @uttryck2.eval
    when ">"
      return @uttryck1.eval > @uttryck2.eval
    when ">="
      return @uttryck1.eval >= @uttryck2.eval
    else
      puts "Något gick åt helvete med jemforelse"
    end
  end
end

#Används för och-uttryck
class Och_nod
  def initialize(uttryck1, uttryck2)
    @uttryck1 = uttryck1
    @uttryck2 = uttryck2
  end

  def eval()
    return (@uttryck1.eval and @uttryck2.eval)
  end
end

#Används för eller-uttryck
class Eller_nod
  def initialize(uttryck1, uttryck2)
    @uttryck1 = uttryck1
    @uttryck2 = uttryck2
  end

  def eval()
    return (@uttryck1.eval or @uttryck2.eval)
  end
end

#Används för skapning av de olika typerna av fragment
class Fragment_nod
  def initialize(v)
    @value = v
  end

  def eval()

    #Specialfall som måste evalueras
    if(@value.class == Behallare_egenskap_nod || @value.class == Boolean_nod)
      return @value.eval
    else
      return @value
    end
  end
end

#Används för retur-uttryck
class Retur_nod
  def initialize(fragment)
    @fragment = fragment
  end

  def eval()

    #Sparar ner värdet som ska returneras så att den kan letas efter då satser utförs
    @@retur_v = @fragment.eval
  end
end

#Används för Skapning av en datatyp
class Datatyp_nod
  def initialize(v)
    @value = v
  end
  
  def eval()
    return @value
  end
end

#Används för skapning av en boolean
class Boolean_nod
  def initialize(v)
    @value = v
  end
  
  def eval()
    if (@value == "sant")
      return true
    else
      return false
    end
  end
end

#Används för skapningen av en fragment-behållare
class Fragment_behallare_nod
  def initialize(v)
    @v = v
  end

  def eval()
    v = @v

    #Går igenom alla delar av behållaren för att ta hand om delen på korrekt sätt
    0.upto(v.length-1) do |i|
      v[i] =  djup_behallare_eval(v[i])
    end
    return v
  end
end

#Används för att hämta ut korrekt värde på en variabel
class Variabel_nod
  def initialize(v)
    @value = v
  end
  
  def eval()
    return hitta_var(@value)
  end
end

#Används för att skicka namnet på en variabel
class Sett_variabel_nod
  def initialize(v)
    @value = v
  end
  
  def eval()
    return @value
  end
end
