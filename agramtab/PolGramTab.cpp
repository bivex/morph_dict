#include "PolGramTab.h"

const static std::string PolishPartOfSpeech[] =
{
	"NOUN",
	"VERB",
	"ADJ",
	"ADV",
	"PRON",
	"PREP",
	"CONJ",
	"INT",
	"NUM",
	"PART",
	"DET"
};

const static std::string PolishGrammems[] =
{
	"sg", "pl",
	"masc", "fem", "neut",
	"vir", "nvir",
	"nom", "gen", "dat", "acc", "ins", "loc", "voc",
	"p1", "p2", "p3",
	"pres", "past", "fut",
	"impf", "pfv",
	"ind", "imp", "cond",
	"inf", "ptcp", "act", "pass",
	"pos", "comp", "sup",
	"anim", "nanim"
};


CPolGramTab::CPolGramTab()
{
	assert(sizeof(PolishGrammems) / sizeof(std::string) == POLISH_GRAMMEMS_COUNT);
	assert(sizeof(PolishPartOfSpeech) / sizeof(std::string) == POLISH_PART_OF_SPEECH_COUNT);

	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		GetLine(i) = NULL;
	m_Language = morphPolish;
}

CPolGramTab::~CPolGramTab()
{
	for (size_t i = 0; i < GetMaxGrmCount(); i++)
		if (GetLine(i) != NULL)
			delete GetLine(i);
}

void CPolGramTab::LoadFromRegistry()
{
	ReadFromFolder(GetDefaultPath());
}

part_of_speech_t CPolGramTab::GetPartOfSpeechesCount() const {
	return POLISH_PART_OF_SPEECH_COUNT;
}

const char* CPolGramTab::GetPartOfSpeechStr(part_of_speech_t i, NamingAlphabet na) const {
	return PolishPartOfSpeech[i].c_str();
}

const char* CPolGramTab::GetPartOfSpeechStrLong(part_of_speech_t i) const {
	return GetPartOfSpeechStr(i);
}

grammem_t CPolGramTab::GetGrammemsCount() const {
	return POLISH_GRAMMEMS_COUNT;
}

const char* CPolGramTab::GetGrammemStr(size_t i, NamingAlphabet na) const {
	return PolishGrammems[i].c_str();
}

size_t CPolGramTab::GetMaxGrmCount() const {
	return eMaxGrmCount;
}

// Polish gender/number agreement: a noun and its modifiers must agree
// in number and (within number) gender class.
bool CPolGramTab::GleicheGenderNumber(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return true;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return true;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	grammems_mask_t number1 = g1 & ((1ULL << polSg) | (1ULL << polPl));
	grammems_mask_t number2 = g2 & ((1ULL << polSg) | (1ULL << polPl));
	if (number1 && number2 && number1 != number2) return false;

	// Singular gender agreement
	grammems_mask_t sg1 = g1 & ((1ULL << polMasc) | (1ULL << polFem) | (1ULL << polNeut));
	grammems_mask_t sg2 = g2 & ((1ULL << polMasc) | (1ULL << polFem) | (1ULL << polNeut));
	if (sg1 && sg2 && sg1 != sg2) return false;

	// Plural gender agreement (virile vs non-virile)
	grammems_mask_t pl1 = g1 & ((1ULL << polVir) | (1ULL << polNvir));
	grammems_mask_t pl2 = g2 & ((1ULL << polVir) | (1ULL << polNvir));
	if (pl1 && pl2 && pl1 != pl2) return false;

	return true;
}

bool CPolGramTab::GleicheSubjectPredicate(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return false;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return false;

	grammems_mask_t g1 = l1->m_Grammems;
	grammems_mask_t g2 = l2->m_Grammems;

	// Polish past tense verbs agree with the subject in number and gender,
	// but present/future agree only in person+number. We apply a relaxed check:
	// number must match if both present.
	grammems_mask_t number1 = g1 & ((1ULL << polSg) | (1ULL << polPl));
	grammems_mask_t number2 = g2 & ((1ULL << polSg) | (1ULL << polPl));
	if (number1 && number2 && number1 != number2) return false;

	// person agreement (present/future)
	grammems_mask_t person1 = g1 & ((1ULL << polP1) | (1ULL << polP2) | (1ULL << polP3));
	grammems_mask_t person2 = g2 & ((1ULL << polP1) | (1ULL << polP2) | (1ULL << polP3));
	if (person1 && person2 && person1 != person2) return false;

	return true;
}

bool CPolGramTab::IsStrongClauseRoot(const part_of_speech_mask_t poses) const
{
	return is_verb_form(poses);
}

bool CPolGramTab::IsMorphNoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << polNOUN)) != 0;
}

bool CPolGramTab::is_morph_adj(part_of_speech_mask_t poses) const
{
	return (poses & (1 << polADJ)) != 0;
}

bool CPolGramTab::is_morph_participle(part_of_speech_mask_t poses) const
{
	return (poses & (1 << polVERB)) != 0;
}

bool CPolGramTab::is_morph_pronoun(part_of_speech_mask_t poses) const
{
	return (poses & (1 << polPRON)) != 0;
}

bool CPolGramTab::is_morph_pronoun_adjective(part_of_speech_mask_t poses) const
{
	return false;
}

bool CPolGramTab::is_left_noun_modifier(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return is_morph_adj(poses);
}

bool CPolGramTab::is_numeral(part_of_speech_mask_t poses) const
{
	return (poses & (1 << polNUM)) != 0;
}

bool CPolGramTab::is_verb_form(part_of_speech_mask_t poses) const
{
	return (poses & (1 << polVERB)) != 0;
}

bool CPolGramTab::is_infinitive(part_of_speech_mask_t poses) const
{
	return false;
}

bool CPolGramTab::is_morph_predk(part_of_speech_mask_t poses) const
{
	return false;
}

bool CPolGramTab::is_morph_adv(part_of_speech_mask_t poses) const
{
	return (poses & (1 << polADV)) != 0;
}

bool CPolGramTab::is_morph_personal_pronoun(part_of_speech_mask_t poses, grammems_mask_t grammems) const
{
	return (poses & (1 << polPRON)) != 0;
}

bool CPolGramTab::IsSimpleParticle(const std::string& lemma, part_of_speech_mask_t poses) const
{
	return false;
}

bool CPolGramTab::IsSynNoun(part_of_speech_mask_t poses, const std::string& lemma) const
{
	return false;
}

bool CPolGramTab::IsStandardParamAbbr(const char* WordStrUpper) const
{
	return false;
}

bool CPolGramTab::GleicheCase(const char* gram_code_noun, const char* gram_code_adj) const
{
	return GleicheCaseNumber(gram_code_noun, gram_code_adj);
}

bool CPolGramTab::GleicheCaseNumber(const char* gram_code1, const char* gram_code2) const
{
	size_t i1 = GramcodeToLineIndex(gram_code1);
	size_t i2 = GramcodeToLineIndex(gram_code2);
	if (i1 >= GetMaxGrmCount() || i2 >= GetMaxGrmCount()) return true;
	const CAgramtabLine* l1 = GetLine(i1);
	const CAgramtabLine* l2 = GetLine(i2);
	if (!l1 || !l2) return true;

	grammems_mask_t case1 = l1->m_Grammems & ((1ULL << polNom) | (1ULL << polGen) | (1ULL << polDat) |
		(1ULL << polAcc) | (1ULL << polIns) | (1ULL << polLoc) | (1ULL << polVoc));
	grammems_mask_t case2 = l2->m_Grammems & ((1ULL << polNom) | (1ULL << polGen) | (1ULL << polDat) |
		(1ULL << polAcc) | (1ULL << polIns) | (1ULL << polLoc) | (1ULL << polVoc));
	if (case1 && case2 && case1 != case2) return false;

	grammems_mask_t number1 = l1->m_Grammems & ((1ULL << polSg) | (1ULL << polPl));
	grammems_mask_t number2 = l2->m_Grammems & ((1ULL << polSg) | (1ULL << polPl));
	if (number1 && number2 && number1 != number2) return false;

	return true;
}

grammems_mask_t CPolGramTab::GleicheGenderNumberCase(const char* common_gram_code_noun, const char* gram_code_noun, const char* gram_code_adj) const
{
	return 0;
}

bool CPolGramTab::PartOfSpeechIsProductive(part_of_speech_t p) const {
	return p == polNOUN || p == polADJ || p == polVERB || p == polADV;
}
