#include "UkrGramTab.h"

struct TMorphConstant {
    std::string Cyrillic;
    std::string Latin;
    std::string CyrillicLong;
};

const static TMorphConstant UkrainianPartOfSpeech[] = {
    {"С", "N", "СУЩЕСТВИТЕЛЬНОЕ"}, 
    {"П", "A", "ПРИЛАГАТЕЛЬНОЕ"}, 
    {"Г", "V", "ЛИЧНАЯ ФОРМА"}, 
    {"МС","PRON", "МЕСТОИМЕНИЕ"}, 
    {"МС-П","PA", "МЕСТОИМЕНИЕ-ПРИЛАГАТЕЛЬНОЕ"}, 
    {"МС-ПРЕДК","P_PRED", "МЕСТОИМЕНИЕ-ПРЕДИКАТИВ"}, 
    {"ЧИСЛ", "NUM", "ЧИСЛИТЕЛЬНОЕ"}, 
    {"ЧИСЛ-П","ORD_NUM", "ПОРЯДКОВОЕ ЧИСЛИТЕЛЬНОЕ"}, 
    {"Н","ADV", "НАРЕЧИЕ"}, 
    {"ПРЕДК", "PRED", "ПРЕДИКАТИВ"}, 
    {"ПРЕДЛ", "PREP", "ПРЕДЛОГ"}, 
    {"ПОСЛ", "POSL", "ПОСЛЕЛОГ"}, 
    {"СОЮЗ", "CONJ", "СОЮЗ"}, 
    {"МЕЖД", "INT", "МЕЖДОМЕТИЕ"}, 
    {"ВВОДН", "INP", "ВВОДНОЕ СЛОВО"},
    {"ФРАЗ", "COLLOC", "ФРАЗЕОЛОГИЗМ"}, 
    {"ЧАСТ", "PARTICLE", "ЧАСТИЦА"}, 
    {"КР_ПРИЛ", "ADJ_SHORT", "КР_ПРИЛ"},  
    {"ПРИЧАСТИЕ", "PARTICIPLE", "ПРИЧАСТИЕ"}, 
    {"ДЕЕПРИЧАСТИЕ","ADV_PARTICIPLE", "ДЕЕПРИЧАСТИЕ"}, 
    {"КР_ПРИЧАСТИЕ","PARTICIPLE_SHORT", "КРАТКОЕ ПРИЧАСТИЕ"}, 
    {"ИНФИНИТИВ", "INFINITIVE", "ИНФИНИТИВ"}  
};

const static TMorphConstant Grammems[] = {
    {"мн", "pl"},
    {"ед", "sg"},
    {"им", "nom"},
    {"рд", "gen"},
    {"дт", "dat"},
    {"вн", "acc"},
    {"тв", "ins"},
    {"пр", "loc"}, 
    {"зв", "voc"},
    {"мр", "mas"},
    {"жр", "fem"},
    {"ср", "neu"},
    {"unused", "unused"},
    {"наст", "pres"},
    {"буд", "fut"},
    {"прош", "past"},
    {"1л", "1p"},
    {"2л", "2p"},
    {"3л", "3p"},
    {"пов", "impe"},
    {"од", "anim"},
    {"но", "inanim"},
    {"сравн", "comp"},
    {"св", "perf"},
    {"нс", "imp"},
    {"нп", "ntr"},
    {"пе", "tr"},
    {"дст", "act"},
    {"стр", "pass"},
    {"0", "ind"},
    {"аббр", "abbr"},
    {"отч", "patr"},
    {"лок", "topo"},
    {"орг", "org"},
    {"кач", "qual"},
    {"дфст", "st"},
    {"вопр", "ques"},
    {"указат", "dem"},
    {"имя", "name"},
    {"фам", "surn"},
    {"безл", "imper"},
    {"жарг", "slang"},
    {"опч", "err"},
    {"разг", "coll"},
    {"притяж", "poss"},
    {"арх", "arch"},
    {"2", "case2"},
    {"поет", "poet"},
    {"проф", "prof"},
    {"прев", "supr"},
    {"полн", "pos"}
};

static std::string uClauseTypes[] =
{
    "ГЛ_ЛИЧН",
    "ДПР",
    "КР_ПРЧ",
    "КР_ПРИЛ",
    "ПРЕДК",
    "ПРЧ",
    "ИНФ",
    "ВВОД",
    "ТИРЕ",
    "НСО",
    "СРАВН",
    "КОПУЛ"
};

CUkrGramTab::CUkrGramTab() : CAgramtab() {
    m_Language = morphUkrainian;
    memset(Lines, 0, sizeof(Lines));
    assert(sizeof(Grammems) / sizeof(TMorphConstant) == UkrainianGrammemsCount);
    assert(sizeof(UkrainianPartOfSpeech) / sizeof(TMorphConstant) == UKRAINIAN_PART_OF_SPEECH_COUNT);
    assert(sizeof(uClauseTypes) / sizeof(std::string) == UKRAINIAN_CLAUSE_TYPE_COUNT);
}

CUkrGramTab::~CUkrGramTab() {
    for (size_t i = 0; i < uMaxGrmCount; i++)
        if (Lines[i]) delete Lines[i];
}

static grammems_mask_t UkrDeduceGrammems(part_of_speech_t POS, grammems_mask_t grammems)
{
    if (POS == uVERB || POS == uADJ_SHORT || POS == uPARTICIPLE_SHORT)
    {
        if (grammems & uAllGenders)
            grammems |= _QM(uSingular);
    }

    if (POS == uNOUN || POS == uADJ_FULL || POS == uPARTICIPLE || POS == uPRONOUN || POS == uPRONOUN_P || POS == uNUMERAL_P)
    {
        if (grammems & uAllGenders)
            grammems |= _QM(uSingular);
    }

    return grammems;
}

static unsigned int UkrCountOfBits(grammems_mask_t n)
{
    unsigned int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

void CUkrGramTab::InitLanguageSpecific(rapidjson::Document& doc) {
    m_PopularGramCodes.m_InanimIndeclNoun = doc.HasMember("inanim_indecl_noun") ? doc["inanim_indecl_noun"].GetString() : "";
    m_PopularGramCodes.m_MasAbbrNoun = doc.HasMember("mas_abbr_noun") ? doc["mas_abbr_noun"].GetString() : "";
    m_PopularGramCodes.m_ProductiveNoun = "";
    m_PopularGramCodes.m_ProductiveSingNoun = "";

    for (uint16_t i = 0; i < GetMaxGrmCount(); i++) {
        auto* l = GetLine(i);
        if (l == nullptr) continue;
        l->m_Grammems = UkrDeduceGrammems(l->m_PartOfSpeech, l->m_Grammems);

        if ((l->m_PartOfSpeech == uNOUN) && (UkrCountOfBits(l->m_Grammems) == 3)) {
            if ((l->m_Grammems & _QM(uVocativ)) == 0) {
                if ((l->m_Grammems & uAllGenders) && (l->m_Grammems & uAllCases)) {
                    if (l->m_Grammems & uAllNumbers) {
                        std::string ancode = LineIndexToGramcode(i);
                        m_PopularGramCodes.m_ProductiveNoun += ancode;
                    }
                    if (l->m_Grammems & _QM(uSingular)) {
                        std::string ancode = LineIndexToGramcode(i);
                        m_PopularGramCodes.m_ProductiveSingNoun += ancode;
                    }
                }
            }
        }
    }
}

void CUkrGramTab::LoadFromRegistry() {
    ReadFromFolder(GetDefaultPath());
}

part_of_speech_t CUkrGramTab::GetPartOfSpeechesCount() const {
    return UKRAINIAN_PART_OF_SPEECH_COUNT;
}

const char* CUkrGramTab::GetPartOfSpeechStr(part_of_speech_t i, NamingAlphabet na) const {
    return (na == naNational) ? UkrainianPartOfSpeech[i].Cyrillic.c_str() : UkrainianPartOfSpeech[i].Latin.c_str();
}

const char* CUkrGramTab::GetPartOfSpeechStrLong(part_of_speech_t i) const {
    return UkrainianPartOfSpeech[i].CyrillicLong.c_str();
}

grammem_t CUkrGramTab::GetGrammemsCount() const {
    return UkrainianGrammemsCount;
}

const char* CUkrGramTab::GetGrammemStr(size_t i, NamingAlphabet na) const {
    return (na == naNational) ? Grammems[i].Cyrillic.c_str() : Grammems[i].Latin.c_str();
}

size_t CUkrGramTab::GetMaxGrmCount() const {
    return uMaxGrmCount;
}

CAgramtabLine*& CUkrGramTab::GetLine(size_t LineNo) {
    if (LineNo >= uMaxGrmCount) return Lines[0];
    return Lines[LineNo];
}

const CAgramtabLine* CUkrGramTab::GetLine(size_t LineNo) const {
    if (LineNo >= uMaxGrmCount) return nullptr;
    return Lines[LineNo];
}

size_t CUkrGramTab::GramcodeToLineIndex(const char* s) const {
    return (unsigned char)s[0] * 0x100 + (unsigned char)s[1] - uStartUp;
}

std::string CUkrGramTab::LineIndexToGramcode(uint16_t i) const {
    uint16_t code = i + uStartUp;
    char s[3];
    s[0] = code / 0x100;
    s[1] = code % 0x100;
    s[2] = 0;
    return s;
}

bool CUkrGramTab::ProcessPOSAndGrammems(const char* tab_str, part_of_speech_t& PartOfSpeech, grammems_mask_t& grammems) const {
    return CAgramtab::ProcessPOSAndGrammems(tab_str, PartOfSpeech, grammems);
}

// --- Ukrainian Gleiche helper functions ---
// Ukrainian has the same case/gender/number system as Russian.
// Cases: nom, gen, dat, acc, ins, loc, voc
// Genders: masc, fem, neut
// Numbers: sg, pl

static bool UkrGleicheGenderNumber(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    return ((uAllNumbers & l1->m_Grammems & l2->m_Grammems) > 0)
        && (((l1->m_Grammems & l2->m_Grammems & _QM(uPlural)) > 0)
            || ((uAllGenders & l1->m_Grammems & l2->m_Grammems) > 0));
}

static bool UkrGleicheCase(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    return (uAllCases & l1->m_Grammems & l2->m_Grammems) > 0;
}

static bool UkrGleicheCaseNumber(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    return ((uAllCases & l1->m_Grammems & l2->m_Grammems) > 0)
        && ((uAllNumbers & l1->m_Grammems & l2->m_Grammems) > 0);
}

static bool UkrGleichePersonNumber(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    return (uAllPersons & l1->m_Grammems & l2->m_Grammems) > 0
        && (uAllNumbers & l1->m_Grammems & l2->m_Grammems) > 0;
}

#include "../common/utilit.h"

static bool UkrGleicheSubjectPredicate(const CAgramtabLine* subj_l, const CAgramtabLine* verb_l) {
    const grammems_mask_t& subj = subj_l->m_Grammems;
    const grammems_mask_t& verb = verb_l->m_Grammems;

    bool res = false;
    if (!(subj & _QM(uNominativ)))
        res = false;
    else if ((verb & _QM(uPastTense)) > 0
        || (uAllGenders & verb) > 0 
        || verb_l->m_PartOfSpeech == uADJ_SHORT
        || verb_l->m_PartOfSpeech == uPARTICIPLE_SHORT)
    {
        if (subj & (_QM(uFirstPerson) | _QM(uSecondPerson)))
            res = (verb & subj & _QM(uPlural)) > 0
                || ((verb & (_QM(uMasculinum) | _QM(uFeminum))) > 0
                    && (verb & subj & _QM(uSingular)) > 0);
        else
            res = UkrGleicheGenderNumber(subj_l, verb_l);
    }
    else if ((verb & _QM(uPresentTense)) > 0 || (verb & _QM(uFutureTense)) > 0
             || (uAllPersons & verb) > 0 
             || (uAllNumbers & verb) > 0) 
    {
        if ((subj & (_QM(uFirstPerson) | _QM(uSecondPerson))) > 0
            || (verb & (_QM(uFirstPerson) | _QM(uSecondPerson))) > 0)
            res = UkrGleichePersonNumber(subj_l, verb_l);
        else
            res = (uAllNumbers & subj & verb) > 0;
    }
    else if (verb & _QM(uImperative)) {
        res = (subj & _QM(uSecondPerson)) > 0
            && (uAllNumbers & subj & verb) > 0;
    }

    if (res) {
        // LOGI << "UkrGleicheSubjectPredicate: SUCCESS"; 
    }
    return res;
}

static bool UkrGleicheGenderNumberCase(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    const grammems_mask_t& g1 = l1->m_Grammems;
    const grammems_mask_t& g2 = l2->m_Grammems;
    return ((uAllCases & g1 & g2) > 0)
        && ((uAllNumbers & g1 & g2) > 0)
        && (((uAllGenders & g1 & g2) > 0)
            || ((uAllGenders & g1) == 0)
            || ((uAllGenders & g2) == 0));
}

static bool UkrGleicheGenderNumberCaseAnim(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    const grammems_mask_t& g1 = l1->m_Grammems;
    const grammems_mask_t& g2 = l2->m_Grammems;
    return ((uAllCases & g1 & g2) > 0)
        && ((uAllNumbers & g1 & g2) > 0)
        && ((_QM(uAnimative) & g2) > 0 || (uAllAnimative & g2) == 0)
        && (((uAllGenders & g1 & g2) > 0)
            || ((uAllGenders & g1) == 0)
            || ((uAllGenders & g2) == 0));
}

static bool UkrGleicheGenderNumberCaseInanim(const CAgramtabLine* l1, const CAgramtabLine* l2) {
    const grammems_mask_t& g1 = l1->m_Grammems;
    const grammems_mask_t& g2 = l2->m_Grammems;
    return ((uAllCases & g1 & g2) > 0)
        && ((uAllNumbers & g1 & g2) > 0)
        && ((_QM(uNonAnimative) & g2) > 0 || (uAllAnimative & g2) == 0)
        && (((uAllGenders & g1 & g2) > 0)
            || ((uAllGenders & g1) == 0)
            || ((uAllGenders & g2) == 0));
}

bool CUkrGramTab::GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const {
    return Gleiche(UkrGleicheCase, gram_code_noun, gram_code_adj) != 0;
}

bool CUkrGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const {
    return Gleiche(UkrGleicheCaseNumber, gram_code1, gram_code2) != 0;
}

grammems_mask_t CUkrGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const {
    if (!common_gram_code_noun || !strcmp(common_gram_code_noun, "??") || strlen(common_gram_code_noun) == 0)
        return Gleiche(UkrGleicheGenderNumberCase, gram_code_noun, gram_code_adj);
    const CAgramtabLine* commonLine = GetLine(GramcodeToLineIndex(common_gram_code_noun));
    if (!commonLine)
        return Gleiche(UkrGleicheGenderNumberCase, gram_code_noun, gram_code_adj);
    else if ((commonLine->m_Grammems & _QM(uNonAnimative)) > 0)
        return Gleiche(UkrGleicheGenderNumberCaseInanim, gram_code_noun, gram_code_adj);
    else if ((commonLine->m_Grammems & _QM(uAnimative)) > 0)
        return Gleiche(UkrGleicheGenderNumberCaseAnim, gram_code_noun, gram_code_adj);
    else
        return Gleiche(UkrGleicheGenderNumberCase, gram_code_noun, gram_code_adj);
}

bool CUkrGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const {
    return Gleiche(UkrGleicheGenderNumber, gram_code1, gram_code2) != 0;
}

bool CUkrGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const {
    return Gleiche(UkrGleicheSubjectPredicate, gram_code1, gram_code2) != 0;
}

long CUkrGramTab::GetClauseTypeByName(const char* TypeName) const {
    for (int i = 0; i < UKRAINIAN_CLAUSE_TYPE_COUNT; i++)
    {
        if (uClauseTypes[i] == TypeName)
        {
            return i;
        }
    }

    return -1;
}

const char* CUkrGramTab::GetClauseNameByType(long type) const {
    if (type >= UKRAINIAN_CLAUSE_TYPE_COUNT) return 0;
    if (type < 0) return 0;

    return uClauseTypes[type].c_str();
}

bool CUkrGramTab::IsStrongClauseRoot(part_of_speech_mask_t poses) const {
    return (poses & (1 << uVERB)) != 0 || (poses & (1 << uPREDK)) != 0 || (poses & (1 << uADJ_SHORT)) != 0 || (poses & (1 << uPARTICIPLE_SHORT)) != 0;
}

bool CUkrGramTab::IsMorphNoun(part_of_speech_mask_t poses) const {
    return (poses & (1 << uNOUN)) != 0;
}

bool CUkrGramTab::is_morph_adj(part_of_speech_mask_t poses) const {
    return (poses & (1 << uADJ_FULL)) != 0 || (poses & (1 << uADJ_SHORT)) != 0;
}

bool CUkrGramTab::is_morph_participle(part_of_speech_mask_t poses) const {
    return (poses & (1 << uPARTICIPLE)) != 0 || (poses & (1 << uPARTICIPLE_SHORT)) != 0;
}

bool CUkrGramTab::is_morph_pronoun(part_of_speech_mask_t poses) const {
    return (poses & (1 << uPRONOUN)) != 0;
}

bool CUkrGramTab::is_morph_pronoun_adjective(part_of_speech_mask_t poses) const {
    return (poses & (1 << uPRONOUN_P)) != 0;
}

bool CUkrGramTab::is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const {
    return is_morph_adj(poses) || is_morph_participle(poses) || is_morph_pronoun_adjective(poses);
}

bool CUkrGramTab::is_numeral(part_of_speech_mask_t poses) const {
    return (poses & (1 << uNUMERAL)) != 0 || (poses & (1 << uNUMERAL_P)) != 0;
}

bool CUkrGramTab::is_verb_form(part_of_speech_mask_t poses) const {
    return (poses & (1 << uVERB)) != 0 || (poses & (1 << uINFINITIVE)) != 0 || (poses & (1 << uADVERB_PARTICIPLE)) != 0;
}

bool CUkrGramTab::is_infinitive(part_of_speech_mask_t poses) const {
    return (poses & (1 << uINFINITIVE)) != 0;
}

bool CUkrGramTab::is_morph_predk(part_of_speech_mask_t poses) const {
    return (poses & (1 << uPREDK)) != 0;
}

bool CUkrGramTab::is_morph_adv(part_of_speech_mask_t poses) const {
    return (poses & (1 << uADV)) != 0;
}

bool CUkrGramTab::is_morph_personal_pronoun(part_of_speech_mask_t poses, grammems_mask_t grammems) const {
    return (poses & (1 << uPRONOUN)) != 0 && (grammems & (_QM(uFirstPerson) | _QM(uSecondPerson) | _QM(uThirdPerson)));
}

bool CUkrGramTab::IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const {
    return (poses & (1 << uPARTICLE)) != 0;
}

bool CUkrGramTab::IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const {
    return IsMorphNoun(poses) ||
           (poses & (1 << uPRONOUN)) != 0 ||
           ((poses & (1 << uPRONOUN_P)) != 0 && 
               (lemma == "КОЖЕН" || 
                lemma == "ОДИН" || 
                lemma == "ІНШИЙ" || 
                lemma == "ТОЙ" || 
                lemma == "ЯКИЙ"));
}

bool CUkrGramTab::IsStandardParamAbbr(const char* WordStrUpper) const {
    return false;
}

bool CUkrGramTab::PartOfSpeechIsProductive(part_of_speech_t p) const {
    return p == uNOUN || p == uADJ_FULL || p == uVERB;
}

std::string CUkrGramTab::FilterGramCodes1(const std::string& gram_codes, grammems_mask_t positive, grammems_mask_t negative) const
{
    std::string result;
    for (size_t l = 0; l < gram_codes.length(); l += 2)
    {
        auto g = GetLine(GramcodeToLineIndex(gram_codes.c_str() + l))->m_Grammems;
        if (((g & positive) == positive) && ((g & negative) == 0)) {
            result.append(gram_codes.c_str() + l, 2);
        }
    }
    return result;
}

static bool UkrCaseNumberGender0(const CAgramtabLine* l1, const CAgramtabLine* l2)
{
    return ((uAllCases & l1->m_Grammems & l2->m_Grammems) > 0 || !(uAllCases & l1->m_Grammems) || !(uAllCases & l2->m_Grammems)) &&
        ((uAllNumbers & l1->m_Grammems & l2->m_Grammems) > 0 || !(uAllNumbers & l1->m_Grammems) || !(uAllNumbers & l2->m_Grammems)) &&
        ((uAllGenders & l1->m_Grammems & l2->m_Grammems) > 0 || !(uAllGenders & l1->m_Grammems) || !(uAllGenders & l2->m_Grammems));
}

bool CUkrGramTab::FilterNounNumeral(std::string& gcNoun, const std::string& gcNum, grammems_mask_t& grammems) const
{
    if (gcNoun.length() == 2 || !(grammems & uAllCases)) return false;
    auto c2 = FilterGramCodes1(gcNum, grammems & uAllCases, 0);
    auto gcNoun2 = GleicheAncode1(UkrCaseNumberGender0, gcNoun, c2);
    gcNoun = gcNoun2;
    return true;
}

grammems_mask_t CUkrGramTab::ChangeGleicheAncode1(GrammemCompare CompareFunc, const std::string& wordGramCodes, std::string& groupGramCodes, const grammems_mask_t wordGrammems) const
{
    auto c2 = FilterGramCodes1(wordGramCodes, GetMaxQWORD(), _QM(uIndeclinable));
    groupGramCodes = GleicheAncode1(CompareFunc, c2, groupGramCodes);
    if (groupGramCodes == "") { return 0; }
    const grammems_mask_t gramFilter = uAllCases | uAllGenders | uAllTimes | uAllPersons | uAllAnimative;
    return  wordGrammems & ~(gramFilter) | GetAllGrammems(groupGramCodes.c_str());
}
