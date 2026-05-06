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

CUkrGramTab::CUkrGramTab() : CAgramtab() {
    m_Language = morphUkrainian;
    memset(Lines, 0, sizeof(Lines));
}

CUkrGramTab::~CUkrGramTab() {
    for (size_t i = 0; i < uMaxGrmCount; i++)
        if (Lines[i]) delete Lines[i];
}

void CUkrGramTab::InitLanguageSpecific(rapidjson::Document& doc) {
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
    return Lines[LineNo];
}

const CAgramtabLine* CUkrGramTab::GetLine(size_t LineNo) const {
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

bool CUkrGramTab::GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const {
    return false; 
}

bool CUkrGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const {
    return false; 
}

grammems_mask_t CUkrGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const {
    return 0; 
}

bool CUkrGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const {
    return false; 
}

bool CUkrGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const {
    return false; 
}

long CUkrGramTab::GetClauseTypeByName(const char* TypeName) const {
    return 0;
}

const char* CUkrGramTab::GetClauseNameByType(long type) const {
    return "";
}

bool CUkrGramTab::IsStrongClauseRoot(part_of_speech_mask_t poses) const {
    return false;
}

bool CUkrGramTab::IsMorphNoun(part_of_speech_mask_t poses) const {
    return (poses & (1 << uNOUN)) != 0;
}

bool CUkrGramTab::is_morph_adj(part_of_speech_mask_t poses) const {
    return (poses & (1 << uADJ_FULL)) != 0 || (poses & (1 << uADJ_SHORT)) != 0;
}

bool CUkrGramTab::is_morph_participle(part_of_speech_mask_t poses) const {
    return (poses & (1 << uPARTICIPLE)) != 0;
}

bool CUkrGramTab::is_morph_pronoun(part_of_speech_mask_t poses) const {
    return (poses & (1 << uPRONOUN)) != 0;
}

bool CUkrGramTab::is_morph_pronoun_adjective(part_of_speech_mask_t poses) const {
    return (poses & (1 << uPRONOUN_P)) != 0;
}

bool CUkrGramTab::is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const {
    return is_morph_adj(poses);
}

bool CUkrGramTab::is_numeral(part_of_speech_mask_t poses) const {
    return (poses & (1 << uNUMERAL)) != 0 || (poses & (1 << uNUMERAL_P)) != 0;
}

bool CUkrGramTab::is_verb_form(part_of_speech_mask_t poses) const {
    return (poses & (1 << uVERB)) != 0 || (poses & (1 << uINFINITIVE)) != 0;
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
    return (poses & (1 << uPRONOUN)) != 0;
}

bool CUkrGramTab::IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const {
    return (poses & (1 << uPARTICLE)) != 0;
}

bool CUkrGramTab::IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const {
    return IsMorphNoun(poses);
}

bool CUkrGramTab::IsStandardParamAbbr(const char* WordStrUpper) const {
    return false;
}

bool CUkrGramTab::PartOfSpeechIsProductive(part_of_speech_t p) const {
    return p == uNOUN || p == uADJ_FULL || p == uVERB;
}
