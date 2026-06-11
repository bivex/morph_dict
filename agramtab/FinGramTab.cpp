#include "FinGramTab.h"

const static std::string FinnishPartOfSpeech[] =
{
	"NOUN",
	"ADJ",
	"VERB",
	"ADV",
	"PRON",
	"PREP",
	"CONJ",
	"INT",
	"NUM",
	"PART",
	"ADP",
	"PROPN"
};

const static std::string FinnishGrammems[] =
{
	"sg", "pl",
	"nom", "gen", "par", "ine", "ela", "ill",
	"ade", "abl", "all", "ess", "tra", "ins", "abs", "com",
	"pres", "past", "imp", "cond", "pot",
	"ind",
	"p1", "p2", "p3",
	"inf", "ptcp",
	"act", "pass",
	"pos", "neg", "prf"
};


CFinGramTab::CFinGramTab()
{
	assert(sizeof(FinnishGrammems) / sizeof(std::string) == FINNISH_GRAMMEMS_COUNT);
	assert(sizeof(FinnishPartOfSpeech) / sizeof(std::string) == FINNISH_PART_OF_SPEECH_COUNT);

	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		GetLine(i) = NULL;
	m_Language = morphFinnish;
}

CFinGramTab::~CFinGramTab()
{
	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		if (GetLine(i) != NULL)
			delete GetLine(i);
}

void CFinGramTab::LoadFromRegistry()
{
	ReadFromFolder(GetDefaultPath());
}

part_of_speech_t CFinGramTab::GetPartOfSpeechesCount() const {
	return FINNISH_PART_OF_SPEECH_COUNT;
}

const char* CFinGramTab::GetPartOfSpeechStr(part_of_speech_t i, NamingAlphabet na) const {
	return FinnishPartOfSpeech[i].c_str();
}

const char* CFinGramTab::GetPartOfSpeechStrLong(part_of_speech_t i) const {
	return GetPartOfSpeechStr(i);
}

grammem_t CFinGramTab::GetGrammemsCount() const {
	return FINNISH_GRAMMEMS_COUNT;
}

const char* CFinGramTab::GetGrammemStr(size_t i, NamingAlphabet na) const {
	return FinnishGrammems[i].c_str();
}

size_t CFinGramTab::GetMaxGrmCount() const {
	return eMaxGrmCount;
}

bool CFinGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return true;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return true;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t number1 = g1 & ((1ULL << finSg) | (1ULL << finPl));
	grammems_mask_t number2 = g2 & ((1ULL << finSg) | (1ULL << finPl));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CFinGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return false;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return false;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t person1 = g1 & ((1ULL << finP1) | (1ULL << finP2) | (1ULL << finP3));
	grammems_mask_t person2 = g2 & ((1ULL << finP1) | (1ULL << finP2) | (1ULL << finP3));
	if (person1 && person2 && person1 != person2) return false;

	grammems_mask_t number1 = g1 & ((1ULL << finSg) | (1ULL << finPl));
	grammems_mask_t number2 = g2 & ((1ULL << finSg) | (1ULL << finPl));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

bool CFinGramTab::IsStrongClauseRoot(const part_of_speech_mask_t poses) const
{
	return is_verb_form(poses);
}

bool CFinGramTab::IsMorphNoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << finNOUN)) != 0;
}

bool CFinGramTab::is_morph_adj(part_of_speech_mask_t poses) const
{
	return (poses & (1 << finADJ)) != 0;
}

bool CFinGramTab::is_morph_participle(part_of_speech_mask_t poses) const
{
	return false;
}

bool CFinGramTab::is_morph_pronoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << finPRON)) != 0;
}

bool CFinGramTab::is_morph_pronoun_adjective(part_of_speech_mask_t poses) const
{
	return false;
}

bool CFinGramTab::is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return is_morph_adj(poses);
}

bool CFinGramTab::is_numeral(part_of_speech_mask_t poses) const
{
	return (poses & (1 << finNUM)) != 0;
}

bool CFinGramTab::is_verb_form(part_of_speech_mask_t poses) const
{
	return (poses & (1 << finVERB)) != 0;
}

bool CFinGramTab::is_infinitive(part_of_speech_mask_t poses) const
{
	return false;
}

bool CFinGramTab::is_morph_predk(part_of_speech_mask_t poses) const
{
	return false;
}

bool CFinGramTab::is_morph_adv(part_of_speech_mask_t poses) const
{
	return (poses & (1 << finADV)) != 0;
}

bool CFinGramTab::is_morph_personal_pronoun(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return (poses & (1 << finPRON)) != 0;
}

bool CFinGramTab::IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const
{
	return false;
}

bool CFinGramTab::IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const
{
	return false;
}

bool CFinGramTab::IsStandardParamAbbr(const char* WordStrUpper) const
{
	return false;
}

bool CFinGramTab::GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const
{
	return false;
}

bool CFinGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const
{
	return false;
}

grammems_mask_t CFinGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const
{
	return 0;
}

bool CFinGramTab::PartOfSpeechIsProductive(part_of_speech_t p) const {
	return p == finNOUN || p == finADJ || p == finVERB || p == finADV;
}
