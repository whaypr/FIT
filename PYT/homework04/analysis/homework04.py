"""
Dataset obsahuje nasledujici promenne:
 'Age' - vek v rocich
 'Fare' - cena jizdenky
 'Name' - jmeno cestujiciho
 'Parch' - # rodicu/deti daneho cloveka na palube
 'PassengerId' - Id
 'Pclass' - Trida, 1 = 1. trida, 2 = 2.trida, 3 = 3.trida
 'Sex' - pohlavi
 'SibSp' - # sourozencu/manzelu daneho cloveka na palube
 'Survived' - 0 = Neprezil, 1 = Prezil
 'Embarked' - Pristav, kde se dany clovek nalodil. C = Cherbourg, Q = Queenstown, S = Southampton
 'Cabin' - Cislo kabiny
 'Ticket' - Cislo tiketu
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


def load_dataset(train_file_path, test_file_path):
    """
    Napiste funkci, ktera nacte soubory ze souboru zadanych parametrem a vytvori dva separatni DataFrame. Pro testovani vyuzijte data 'data/train.csv' a 'data/test.csv'
    Ke kazdemu dataframe pridejte sloupecek pojmenovaný jako "Label", ktery bude obsahovat hodnoty "Train" pro train.csv a "Test" pro test.csv.

    1. Pote slucte oba dataframy.
    2. Z vysledneho slouceneho DataFramu odstraňte sloupce  "Ticket", "Embarked", "Cabin".
    3. Sloučený DataDrame bude mít index od 0 do počtu řádků.
    4. Vratte slouceny DataFrame.
    """

    train = pd.read_csv(train_file_path)
    test = pd.read_csv(test_file_path)

    train['Label'] = 'Train'
    test['Label'] = 'Test'

    res = pd.concat([train, test])
    res = (
        res
        .drop(['Ticket', 'Embarked', 'Cabin'], axis='columns')
        .set_index( pd.Index( range( len( res.index ) ) ) )
    )

    return res


def get_missing_values(df : pd.DataFrame) -> pd.DataFrame:
    """
    Ze zadaneho dataframu zjistete chybejici hodnoty. Vytvorte DataFrame, ktery bude obsahovat v indexu jednotlive promenne
    a v prvnim sloupci bude promenna 'Total' obsahujici celkovy pocet chybejicich hodnot a ve druhem sloupci promenna 'Percent',
    ve ktere bude procentualni vyjadreni chybejicich hodnot vuci celkovemu poctu radku v tabulce.
    DataFrame seradte od nejvetsich po nejmensi hodnoty.
    Vratte DataFrame chybejicich hodnot.

    Priklad:

               |  Total  |  Percent
    "Column1"  |   34    |    76
    "Column2"  |   0     |    0

    """

    number_of_entries = len(df.index)
    missing = pd.DataFrame(index=df.columns)

    for col in df.columns:
        null_count = df[col].isnull().sum()
        missing.loc[col, 'Total'] = null_count
        missing.loc[col, 'Percent'] = null_count / number_of_entries * 100
    
    missing = missing.sort_values('Total', ascending=False)

    return missing


def substitute_missing_values(df: pd.DataFrame) -> pd.DataFrame:
    """
    Chybejici hodnoty ve sloupecku "Age" nahradte meanem hodnot z "Age".
    Chybejici hodnoty ve sloupecku "Fare" nahradte meadianem hodnot z "Fare".
    V jednom pripade pouzijte "loc" a ve druhem "fillna".
    Zadany DataFrame neupravujte, ale vytvorte si kopii.
    Vratte upraveny DataFrame.
    """

    df_new = df.copy()

    df_new.loc[df.Age.isnull(), 'Age'] = df.Age.mean()
    df_new['Fare'].fillna(df_new.Fare.median(), inplace = True)

    return df_new


def get_correlation(df: pd.DataFrame) -> float:
    """
    Spocitejte korelaci pro "Age" a "Fare" a vratte korelaci mezi "Age" a "Fare".
    """

    return df['Age'].corr(df['Fare'])


def get_survived_per_class(df : pd.DataFrame, group_by_column_name : str) ->pd.DataFrame:
    """
    Spocitejte prumer z promenne "Survived" pro kazdou skupinu zadanou parametrem "group_by_column_name".
    Hodnoty seradte od nejvetsich po mejmensi.
    Hodnoty "Survived" zaokhroulete na 2 desetinna mista.
    Vratte pd.DataFrame.

    Priklad:

    get_survived_per_class(df, "Sex")

                 Survived
    Female   |      0.82
    Male     |      0.32

    """

    df_group = df.groupby(group_by_column_name)

    survived = (
        df_group['Survived']
        .mean()
        .sort_values(ascending=False)
        .round(2)
        .to_frame()
        .reset_index()
    )

    return survived


def get_outliers(df: pd.DataFrame) -> (int, str):
    """
    Vyfiltrujte odlehle hodnoty (outliers) ve sloupecku "Fare" pomoci metody IRQ.
    Tedy spocitejte rozdil 3. a 1. kvantilu, tj. IQR = Q3 - Q1.
    Pote odfiltrujte vsechny hodnoty nesplnujici: Q1 - 1.5*IQR < "Fare" < Q3 + 1.5*IQR.
    Namalujte box plot pro sloupec "Fare" pred a po vyfiltrovani outlieru.
    Vratte tuple obsahujici pocet outlieru a jmeno cestujiciho pro nejvetsi outlier.
    """

    df['Fare'].plot.box();
    plt.show()

    quartile3 = df.Fare.quantile(0.75)
    quartile1 = df.Fare.quantile(0.25)
    iqr = quartile3 - quartile1

    is_fare_outlier_condition = (
        (df.Fare < quartile1 - 1.5 * iqr) |
        (df.Fare > quartile3 + 1.5 * iqr)
    )

    res = df[is_fare_outlier_condition]

    res['Fare'].plot.box();
    plt.show()

    return res['Fare'].count(), res.loc[res['Fare'].idxmax(), 'Name']


def normalise(df: pd.DataFrame, col: str) -> pd.DataFrame:
    """
    Naskalujte sloupec "col" zadany parametrem pro kazdou "Pclass" hodnotu z dataframu "df" zvlast.
    Pouzijte vzorec: scaled_x_i = (x_i - min(x)) / (max(x) - min(x)), kde "x_i" prestavuje konkretni hodnotu ve sloupeci "col".
    Vratte naskalovany dataframe.
    """

    def min_max_scale(x: pd.Series) -> pd.Series:
        minimum = x.min()
        maximum = x.max()
        
        return (x - minimum) / (maximum - minimum)

    df_col_normalise = df.copy()

    df_col_normalise[col] = df_col_normalise.groupby('Pclass')[col].apply(min_max_scale)

    return df_col_normalise


def create_new_features(df: pd.DataFrame) -> pd.DataFrame:
    """
    Vytvorte 3 nove promenne:
    1. "Fare_scaled" - vytvorte z "Fare" tak, aby mela nulovy prumer a jednotkovou standartni odchylku.
    2. "Age_log" - vytvorte z "Age" tak, aby nova promenna byla logaritmem puvodni "Age".
    3. "Sex" - Sloupec "Sex" nahradte: "female" -> 1, "male" -> 0, kde 0 a 1 jsou integery.

    Nemodifikujte predany DataFrame, ale vytvorte si novy, upravte ho a vratte jej.
    """

    df_new = df.copy()

    df_new['Fare_scaled'] = df_new['Fare'] - np.mean(df_new['Fare'])
    df_new['Fare_scaled'] /= np.std(df_new['Fare_scaled']) 

    df_new['Age_log'] = np.log(df_new['Age'])
    df_new['Sex'] = df_new['Sex'].replace(['female', 'male'], [1, 0])

    return df_new


def determine_survival(df: pd.DataFrame, n_interval: int, age: float, sex: str) -> float:
    """
    Na zaklade statistickeho zpracovani dat zjistete pravdepodobnost preziti Vami zadaneho cloveka (zadava se vek a pohlavi pomoci parametru "age" a "sex")

    Vsechny chybejici hodnoty ve vstupnim DataFramu ve sloupci "Age" nahradte prumerem.
    Rozdelte "Age" do n intervalu zadanych parametrem "n_interval". Napr. pokud bude Age mit hodnoty [2, 13, 18, 25] a mame jej rozdelit do 2 intervalu,
    tak bude vysledek:

    0    (1.977, 13.5]
    1    (1.977, 13.5]
    2     (13.5, 25.0]
    3     (13.5, 25.0]

    Pridejte k rozdeleni jeste pohlavi. Tj. pro kazdou kombinaci pohlavi a intervalu veku zjistete prumernou
    pravdepodobnost preziti ze sloupce "Survived" a tu i vratte.

    Vysledny DataFrame:

    "AgeInterval"   |    "Sex"    |   "Survival Probability"
       (0-10)       | "male"      |            0.21
       (0-10)       | "female"    |            0.28
       (10-20)      | "male"      |            0.10
       (10-20)      | "female"    |            0.15
       atd...

    Takze vystup funkce determine_survival(df, n_interval=20, age = 5, sex = "male") bude 0.21. Tato hodnota bude navratovou hodnotou funkce.

    """
    
    df_new = df.copy()

    df_new.loc[df_new.Age.isnull(), 'Age'] = df_new.Age.mean()

    val = (df_new.Age.max() - df_new.Age.min()) / n_interval
    lower = df_new.Age.min() - 0.03
    upper = lower + val + 0.03

    while age <= lower or age > upper:
        lower = upper
        upper += val

    lower, upper

    df_new.loc[(df.Age > lower) & (df_new.Age <= upper), 'AgeInterval'] = f'({lower}, {upper})'

    df_group = df_new.groupby(['AgeInterval', 'Sex'])

    survived = df_group['Survived'].mean().to_frame().reset_index()

    if survived[survived.Sex == sex].Survived.empty:
        return 0.0

    return survived[survived.Sex == sex].Survived.item()
